# 🌾 CORNTECH: Sistem Monitoring Suhu dan Kelembaban Terintegrasi IoT dan Blockchain untuk Penyimpanan Aman dan Berkualitas Komoditas Pertanian

## 📘 Deskripsi Proyek

**CORNTECH** adalah sistem monitoring suhu dan kelembaban real-time berbasis IoT yang dirancang untuk meningkatkan kualitas dan keamanan penyimpanan komoditas pertanian, khususnya **jagung**. Dengan pendekatan teknologi **IoT** dan **Blockchain**, sistem ini menjamin **transparansi**, **integritas data**, dan **efisiensi dalam rantai pasok pertanian**.

Teknologi yang digunakan mencakup sensor industri SHT20 (Modbus), penyimpanan data time-series menggunakan **InfluxDB**, visualisasi melalui **Grafana** dan **Qt GUI**, serta pencatatan hash data ringkasan ke jaringan **Ethereum Blockchain**.

## 🧠 Tujuan

- Menjaga kualitas komoditas pertanian selama masa penyimpanan
- Mengurangi potensi kerugian akibat suhu/kelembaban yang tidak terkontrol
- Meningkatkan transparansi dan kepercayaan antar pelaku rantai pasok
- Mendukung transformasi menuju **Agriculture 4.0**

---

## 🎓 Informasi Akademik

- **Mata Kuliah**: Interkoneksi Sistem Instrumentasi – VI231418  
- **Dosen Pengampu**: Ahmad Radhy, S.Si., M.Si.  
- **Program Studi**: Rekayasa Teknologi Instrumentasi – Fakultas Vokasi ITS

### 👥 Anggota Tim

| Nama | NRP |
|------|-----|
| Andre Mahesa Bagaskara | 2042231012 |
| Cahyo Okto Risfian | 2042231044 |
| Siti Aisyah | 2042231062 |

---

## ⚙️ Fitur Utama

- ✅ **Monitoring suhu & kelembaban** menggunakan sensor SHT20 via Modbus RTU/TCP
- ✅ **Penyimpanan data historis** dengan InfluxDB
- ✅ **Visualisasi interaktif** dengan Grafana dan Qt GUI
- ✅ **Pencatatan hash ringkasan data** ke Ethereum Blockchain
- ✅ **Web3 DApp** untuk verifikasi data oleh publik

---

## 🛠️ Arsitektur Sistem

```
[SHT20 Sensor] --> [Modbus Client (Rust)] --> [TCP Server (Rust)] --> [InfluxDB] --> [Grafana / Qt GUI]
                                                                      |
                                                                      +--> [Ethereum Blockchain + DApp]
```

---

## 📦 Implementasi

📁 Struktur kode terdapat dalam repositori dengan penjelasan masing-masing komponen.

### 💻 Modbus Client (Rust)
- Membaca data dari sensor SHT20 melalui Modbus RTU
- Mengemas data menjadi format JSON
- Mengirimkan ke TCP Server

### 💻 TCP Server (Rust)
- Menerima data dari Modbus Client
- Parsing dan simpan data ke InfluxDB
- Mengelola hash ringkasan data untuk dicatat ke Ethereum

### 💾 InfluxDB + Grafana
- Menyimpan dan menampilkan data historis dan real-time

### 🔗 Blockchain + DApp
- Hash ringkasan data dicatat ke Ethereum
- DApp berbasis Web3 digunakan untuk verifikasi publik

---

## 📊 Hasil & Analisis

- 🌡️ Data suhu dan kelembaban berhasil dibaca dan disimpan di InfluxDB
- 📈 Visualisasi data real-time menggunakan Grafana dan Qt GUI
- 🔐 Hash data tercatat di Ethereum Blockchain dan diverifikasi melalui DApp

---

## 🚀 Pengembangan Selanjutnya

- 🤖 Integrasi AI untuk prediksi kualitas penyimpanan & rekomendasi aksi
- 🎥 Kamera mini untuk monitoring visual gudang
- 🛰️ Integrasi GPS dan IoT untuk distribusi multi-lokasi
- 🛒 Dashboard marketplace internal untuk rantai pasok pertanian

---

## 📝 Lisensi

CORNTECH_TEKINS23_ITS  
Hak Cipta © 2025 - Tim CORNTECH

---

## 🏷️ Slogan

> “Simpan Lebih Aman, Panen Lebih Terjamin — Transformasi Digital untuk Komoditas Pertanian Indonesia.”
