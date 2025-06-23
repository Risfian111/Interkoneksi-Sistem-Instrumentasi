use tokio::{
    io::AsyncReadExt,
    net::TcpListener,
};
use influxdb2::{Client, models::DataPoint};
use serde::Deserialize;
use futures::stream;
use chrono::{Utc, DateTime};
use std::env;
use dotenvy::dotenv;
use anyhow::Result;

#[derive(Debug, Deserialize)]
struct SensorData {
    timestamp: String,
    sensor_id: String,
    location: String,
    process_stage: String,
    temperature: f64,
    humidity: f64,
}

#[tokio::main]
async fn main() -> Result<()> {
    dotenv().ok();

    let influxdb_url = env::var("INFLUXDB_URL").unwrap_or_else(|_| "http://localhost:8086".into());
    let influxdb_token = env::var("INFLUXDB_TOKEN").unwrap_or_else(|_| "kfVeQsmcnkKXFABXQr9-TJSZ3G_Wz2Xo4kmTDgFlwo-jvCTUyltw6NTqule_MOZNZdsVQfieX80g-Ycz76dcSA==".into());
    let org = env::var("INFLUXDB_ORG").unwrap_or_else(|_| "Interkoneksi".into());
    let bucket = env::var("INFLUXDB_BUCKET").unwrap_or_else(|_| "ISI".into());

    let client = Client::new(influxdb_url.clone(), org.clone(), influxdb_token.clone());

    // Cek koneksi InfluxDB
    let health = client.health().await?;
    if health.status != influxdb2::models::Status::Pass {
        eprintln!("❌ InfluxDB tidak sehat: {:?}", health.status);
        return Ok(());
    }
    println!("✅ Terhubung ke InfluxDB: {:?}", health.status);

    // Jalankan TCP server
    let listener = TcpListener::bind("0.0.0.0:7979").await?;
    println!("📡 TCP Server berjalan di port 7979...");

    loop {
        let (mut socket, addr) = listener.accept().await?;
        println!("🔌 Koneksi dari {}", addr);

        let client = client.clone();
        let bucket = bucket.clone();
        let org = org.clone();

        tokio::spawn(async move {
            let mut buffer = vec![0; 1024];
            match socket.read(&mut buffer).await {
                Ok(n) if n > 0 => {
                    let data = &buffer[..n];
                    println!("📥 Data masuk: {}", String::from_utf8_lossy(data));

                    let sensor_data: SensorData = match serde_json::from_slice(data) {
                        Ok(d) => d,
                        Err(e) => {
                            eprintln!("❌ Gagal parsing JSON: {}", e);
                            return;
                        }
                    };

                    println!("✅ Parsed SensorData: {:?}", sensor_data);

                    let timestamp = DateTime::parse_from_rfc3339(&sensor_data.timestamp)
                        .map(|dt| dt.timestamp_nanos())
                        .unwrap_or_else(|_| Utc::now().timestamp_nanos());

                    let point = DataPoint::builder("sensor_data")
                        .tag("sensor_id", sensor_data.sensor_id)
                        .tag("location", sensor_data.location)
                        .tag("process_stage", sensor_data.process_stage)
                        .field("temperature", sensor_data.temperature)
                        .field("humidity", sensor_data.humidity)
                        .timestamp(timestamp)
                        .build()
                        .unwrap();

                    println!("📤 Menulis ke InfluxDB -> Org: {}, Bucket: {}", org, bucket);

                    if let Err(e) = client.write(&bucket, stream::once(async { point })).await {
                        eprintln!("❌ Gagal menulis ke InfluxDB: {}", e);
                    } else {
                        println!("✅ Data berhasil ditulis ke InfluxDB.");
                    }
                }
                Ok(_) => println!("⚠️ Tidak ada data diterima."),
                Err(e) => eprintln!("❌ Kesalahan saat membaca socket: {}", e),
            }
        });
    }
}
