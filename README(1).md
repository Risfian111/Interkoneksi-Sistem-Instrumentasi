
# Tugas Interkoneksi Sistem Instrumentasi 






## TCP Server
main.rs
``` 
use chrono::{DateTime, Utc};
use dotenvy::dotenv;
use influxdb2::{Client, models::DataPoint};
use serde::Deserialize;
use std::{env, time::Duration};
use tokio::{net::TcpListener, io::{AsyncReadExt, AsyncWriteExt}};
use futures::stream;
use modbus::{Client as ModbusClient, rtu};

#[derive(Debug, Deserialize)]
struct SensorData {
    timestamp: String,
    sensor_id: String,
    location: String,
    process_stage: String,
    temperature_celsius: f64,
    humidity_percent: f64,
}

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    dotenv().ok();

    let influx_url = env::var("INFLUXDB_URL")?;
    let influx_org = env::var("INFLUXDB_ORG")?;
    let influx_token = env::var("INFLUXDB_TOKEN")?;
    let influx_bucket = env::var("INFLUXDB_BUCKET")?;

    let client = Client::new(&influx_url, &influx_org, &influx_token);
    client.health().await?;

    println!("✅ InfluxDB OK. Starting...");

    let client_modbus = client.clone();
    let influx_bucket_modbus = influx_bucket.clone();

    tokio::spawn(async move {
        loop {
            match read_sensor_data("/dev/ttyUSB0", 1) {
                Ok((temperature, humidity)) => {
                    let timestamp = Utc::now().timestamp_nanos_opt().unwrap_or(0);
                    let point = DataPoint::builder("environment_monitoring")
                        .tag("sensor_id", "SHT40-01")
                        .tag("location", "Gudang A")
                        .tag("process_stage", "fermentasi")
                        .field("temperature_celsius", temperature)
                        .field("humidity_percent", humidity)
                        .timestamp(timestamp)
                        .build()
                        .unwrap();

                    match client_modbus.write(&influx_bucket_modbus, stream::iter(vec![point])).await {
                        Ok(_) => println!("📡 Modbus data written to InfluxDB"),
                        Err(e) => eprintln!("❌ Modbus write error: {}", e),
                    }
                }
                Err(e) => eprintln!("❌ Modbus read error: {}", e),
            }

            tokio::time::sleep(Duration::from_secs(10)).await;
        }
    });

    // TCP Server
    let client_tcp = client.clone();
    let influx_bucket_tcp = influx_bucket.clone();
    let listener = TcpListener::bind("127.0.0.1:7878").await?;
    println!("🔌 TCP Server listening on 127.0.0.1:7878");

    loop {
        let (mut socket, _) = listener.accept().await?;
        let client = client_tcp.clone();
        let bucket = influx_bucket_tcp.clone();

        tokio::spawn(async move {
            let mut buf = [0; 1024];
            loop {
                let n = match socket.read(&mut buf).await {
                    Ok(0) => break,
                    Ok(n) => n,
                    Err(e) => {
                        eprintln!("Socket error: {}", e);
                        break;
                    }
                };

                let data = match std::str::from_utf8(&buf[..n]) {
                    Ok(d) => d,
                    Err(_) => {
                        let _ = socket.write_all(b"ERROR: Invalid UTF-8").await;
                        continue;
                    }
                };

                match serde_json::from_str::<SensorData>(data) {
                    Ok(sensor) => {
                        let timestamp = match DateTime::parse_from_rfc3339(&sensor.timestamp) {
                            Ok(dt) => dt.with_timezone(&Utc).timestamp_nanos_opt().unwrap_or(0),
                            Err(_) => {
                                let _ = socket.write_all(b"ERROR: Timestamp format").await;
                                continue;
                            }
                        };

                        let point = DataPoint::builder("environment_monitoring")
                            .tag("sensor_id", &sensor.sensor_id)
                            .tag("location", &sensor.location)
                            .tag("process_stage", &sensor.process_stage)
                            .field("temperature_celsius", sensor.temperature_celsius)
                            .field("humidity_percent", sensor.humidity_percent)
                            .timestamp(timestamp)
                            .build()
                            .unwrap();

                        match client.write(&bucket, stream::iter(vec![point])).await {
                            Ok(_) => {
                                let _ = socket.write_all(b"OK").await;
                                println!("📥 TCP data written to InfluxDB");
                            }
                            Err(e) => {
                                let _ = socket.write_all(b"ERROR: InfluxDB write").await;
                                eprintln!("❌ TCP write error: {}", e);
                            }
                        }
                    }
                    Err(e) => {
                        let _ = socket.write_all(b"ERROR: JSON format").await;
                        eprintln!("❌ TCP parse error: {}", e);
                    }
                }
            }
        });
    }
}

// ✅ Modbus RTU read
fn read_sensor_data(port: &str, slave: u8) -> Result<(f64, f64), Box<dyn std::error::Error + Send + Sync>> {
    let mut ctx = rtu::connect_slave(port, slave)?;
    let regs = ctx.read_input_registers(0x0000, 2)?; // Ganti offset jika beda
    let temp = regs[0] as f64 / 100.0;
    let humid = regs[1] as f64 / 100.0;
    Ok((temp, humid))
}

```

cargo.toml

```
[package]
name = "tcp_server"
version = "0.1.0"
edition = "2021"

[dependencies]
tokio = { version = "1", features = ["full"] }
serde = { version = "1.0", features = ["derive"] }
serde_json = "1.0"        # Tambahkan ini
influxdb2 = "0.5"
chrono = { version = "0.4", features = ["serde"] }
futures = "0.3"
dotenvy = "0.15"
modbus = "1.1.1"

```

.env

```
INFLUXDB_URL=http://localhost:8086
INFLUXDB_ORG=BLITAR
INFLUXDB_TOKEN=UuSq6wTkE1tknOR5zYAVWxij-yzvUlrnokSq9LFlxG5HTysteFX6bHxg_rhTZudZGwwNZ0Y9BJlPdmDhY9cOnw==
INFLUXDB_BUCKET=Sensor_Temperature

```

## SHT20

main.rs

```
use chrono::{Local, SecondsFormat};
use tokio_modbus::{client::rtu, prelude::*};
use tokio_serial::SerialStream;
use tokio::{
    net::TcpStream,
    time::{sleep, Duration},
    io::{AsyncReadExt, AsyncWriteExt},
};
use serde_json::json;
use std::error::Error;

async fn sht20(slave: u8) -> Result<Vec<u16>, Box<dyn std::error::Error>> {
    let port_settings = tokio_serial::new("/dev/ttyUSB0", 9600)
        .parity(tokio_serial::Parity::None)
        .stop_bits(tokio_serial::StopBits::One)
        .data_bits(tokio_serial::DataBits::Eight)
        .timeout(Duration::from_secs(1));
    
    let port = SerialStream::open(&port_settings)?;
    let slave = Slave(slave);
    let mut ctx = rtu::attach_slave(port, slave);
    let response = ctx.read_input_registers(1, 2).await?;

    Ok(response)
}


async fn send_to_server(
    sensor_id: &str,
    location: &str,
    process_stage: &str,
    temperature: f32,
    humidity: f32,
    timestamp: chrono::DateTime<Local>,
) -> Result<(), Box<dyn Error>> {
    let mut stream = TcpStream::connect("127.0.0.1:7878").await?;

    let payload = json!({
        "timestamp": timestamp.to_rfc3339_opts(SecondsFormat::Secs, true),
        "sensor_id": sensor_id,
        "location": location,
        "process_stage": process_stage,
        "temperature_celsius": temperature,
        "humidity_percent": humidity
    });

    let json_str = payload.to_string();
    println!("Sending JSON: {}", json_str);

    // Send JSON payload
    stream.write_all(json_str.as_bytes()).await?;
    stream.write_all(b"\n").await?; 

    // Read response
    let mut buf = [0; 1024];
    let n = stream.read(&mut buf).await?;
    println!("Server response: {}", std::str::from_utf8(&buf[..n])?);

    Ok(())
}

#[tokio::main]
async fn main() -> Result<(), Box<dyn Error>> {
    let sensor_id = "SHT20-PascaPanen-001";
    let location = "Gudang Fermentasi 1";
    let process_stage = "Fermentasi";

    loop {
        let timestamp = Local::now();

        match sht20(1).await {
            Ok(response) if response.len() == 2 => {
                let temp = response[0] as f32 / 10.0;
                let rh = response[1] as f32 / 10.0;

                println!(
                    "[{}] {} - {}: Temp = {:.1}°C, RH = {:.1}%",
                    timestamp.format("%Y-%m-%d %H:%M:%S"),
                    location,
                    process_stage,
                    temp,
                    rh
                );

                if let Err(e) = send_to_server(
                    sensor_id,
                    location,
                    process_stage,
                    temp,
                    rh,
                    timestamp,
                )
                .await
                {
                    eprintln!("Failed to send data: {}", e);
                }
            }
            Ok(invalid) => eprintln!("Invalid sensor response: {:?}", invalid),
            Err(e) => eprintln!("Sensor read error: {}", e),
        }

        sleep(Duration::from_secs(10)).await;
    }
}

```
cargo.toml

```
cargo.toml

[package]
name = "sht20"
version = "0.1.0"
edition = "2021"

[dependencies]
chrono = "0.4"
serde_json = "1.0"
tokio = { version = "1.0", features = ["full"] }
tokio-modbus = "0.9"
tokio-serial = "5.4"

```






