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
    let mut stream = TcpStream::connect("127.0.0.1:7979").await?;

    let payload = json!({
        "timestamp": timestamp.to_rfc3339_opts(SecondsFormat::Secs, true),
        "sensor_id": sensor_id,
        "location": location,
        "process_stage": process_stage,
        "temperature": temperature,
        "humidity": humidity
    });

    let json_str = payload.to_string();
    println!("📤 Sending JSON: {}", json_str);

    stream.write_all(json_str.as_bytes()).await?;
    stream.write_all(b"\n").await?;

    let mut buf = [0; 1024];
    if let Ok(n) = stream.read(&mut buf).await {
        if n > 0 {
            println!("📥 Server response: {}", std::str::from_utf8(&buf[..n])?);
        }
    }

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
                    "[{}] ✅ Data Modbus - {}: Temp = {:.1}°C, RH = {:.1}%",
                    timestamp.format("%Y-%m-%d %H:%M:%S"),
                    location,
                    temp,
                    rh
                );

                println!("📡 Mengirim data ke TCP server...");
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
                    eprintln!("❌ Gagal mengirim data: {}", e);
                }
            }
            Ok(invalid) => eprintln!("⚠️ Respon Modbus tidak valid: {:?}", invalid),
            Err(e) => eprintln!("❌ Gagal membaca sensor Modbus: {}", e),
        }

        sleep(Duration::from_secs(10)).await;
    }
}
