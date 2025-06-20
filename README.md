🌾 CORNTECH: Sistem Monitoring Suhu dan Kelembaban Terintegrasi IoT dan Blockchain untuk Penyimpanan Aman dan Berkualitas Komoditas Pertanian 🌾
💡 Inovasi Presisi untuk Gudang Jagung — Transparansi, Keamanan, dan Efisiensi Rantai Pasok 💡

📘 Deskripsi Proyek
Sistem ini dikembangkan untuk mendukung penyimpanan komoditas pertanian, khususnya jagung, agar tetap aman, berkualitas, dan memenuhi standar pangan. Dengan memanfaatkan teknologi Internet of Things (IoT), sistem membaca suhu dan kelembaban gudang secara real-time menggunakan sensor industri (SHT20/Modbus).

🌐 Data lingkungan disimpan secara historis di InfluxDB, divisualisasikan melalui Grafana dan Qt GUI, dan dicatat dalam bentuk hash ringkasan ke Ethereum Blockchain guna menjamin integritas, transparansi, dan akuntabilitas data.

🚜 Sistem ini mendukung transformasi Agriculture 4.0, memperkuat daya saing produk lokal, mengurangi kerugian pascapanen, dan mendorong kepercayaan antar pelaku rantai pasok, mulai dari petani hingga konsumen.

🎓 Mata Kuliah
🛠 Interkoneksi Sistem Instrumentasi – VI231418
👨‍🏫 Dosen Pengampu: Ahmad Radhy, S.Si., M.Si.
🏫 Program Studi Rekayasa Teknologi Instrumentasi – Fakultas Vokasi ITS

👥 Anggota Tim

👤 Nama	🎓 NRP
Andre Mahesa Bagaskara	2042231012
Cahyo Okto Risfian	2042231044
Siti Aisyah	2042231062

⚙️ Fitur Utama Sistem
✅ Monitoring suhu & kelembaban real-time dengan sensor SHT20 berbasis Modbus RTU/TCP
✅ Penyimpanan data historis dengan InfluxDB (efisien dan mendukung query time-series)
✅ Visualisasi data interaktif melalui dashboard Grafana 🌐 dan Qt GUI 💻
✅ Pencatatan hash ringkasan data ke Ethereum Blockchain untuk integritas & transparansi data
✅ DApp Web3 untuk verifikasi publik data ringkasan lingkungan gudang

🛠️ Implementasi & Kode
💻 Modbus Client (Rust) — Membaca data sensor, mengemas dalam JSON, kirim ke TCP Server
💻 TCP Server (Rust) — Menerima data, parsing, menyimpan ke InfluxDB, mengelola hash untuk blockchain
💾 InfluxDB + Grafana — Menyimpan dan menampilkan data secara real-time dan historis
🔗 Blockchain + DApp — Hash data dicatat ke Ethereum, diverifikasi publik lewat Web3

📚 Hasil & Analisa
🌡️ Hasil pembacaan suhu & kelembaban selama penyimpanan
📈 Data tersimpan di InfluxDB & divisualisasikan real-time di Grafana
🌐 Hash ringkasan data tercatat di Blockchain, diverifikasi via DApp

🚀 Saran Pengembangan Selanjutnya
🤖 Integrasi AI untuk prediksi kualitas penyimpanan dan rekomendasi tindakan
🎥 Penambahan kamera mini untuk monitoring visual gudang
🛰️ Integrasi GPS + IoT untuk sistem distribusi multi-lokasi
🛒 Pembuatan dashboard marketplace internal untuk rantai pasok

🌟 Lisensi
📌 CORNTECH_TEKINS23_ITS

🏷️ “Simpan Lebih Aman, Panen Lebih Terjamin — Transformasi Digital untuk Komoditas Pertanian Indonesia.”
