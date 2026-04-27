# Smart Cooling System (Sistem Pendingin Pintar)

Proyek ini adalah simulasi sistem pendingin otomatis berbasis Arduino Uno yang mengatur kecepatan kipas (DC Motor) secara dinamis berdasarkan suhu lingkungan menggunakan sensor TMP36. Sistem ini dilengkapi dengan antarmuka layar LCD, indikator visual (LED), alarm suara (Buzzer), dan fitur keselamatan (Emergency Stop).

Proyek ini disimulasikan menggunakan **Tinkercad**.
<img width="1058" height="555" alt="image" src="https://github.com/user-attachments/assets/a04139c1-1a21-4448-9f05-239a4401fa2a" />

Demo:
https://github.com/user-attachments/assets/3eb2318c-4f91-4a38-8acf-a7190fd3610f

## Fitur Utama

- **Kontrol Suhu Dinamis**: Kecepatan kipas (PWM) menyesuaikan secara otomatis dengan suhu yang dibaca oleh sensor.
- **Konversi Satuan Suhu**: Dilengkapi tombol tekan untuk mengubah tampilan suhu di LCD antara Celsius (°C) dan Fahrenheit (°F).
- **Indikator Visual & Audio**: Menggunakan LED 3 warna untuk menunjukkan status operasional dan buzzer sebagai alarm peringatan saat suhu terlalu panas (Overheating).
- **Sistem Darurat (Emergency Stop)**: Menggunakan fitur *Hardware Interrupt* untuk menghentikan seluruh sistem secara instan jika terjadi keadaan darurat.
- **Layar LCD I2C**: Menampilkan suhu secara *real-time* dan status kecepatan kipas.

## Komponen yang Digunakan

| Komponen | Jumlah | Keterangan |
| :--- | :---: | :--- |
| Arduino Uno R3 | 1 | Mikrokontroler utama |
| Sensor Suhu TMP36 | 1 | Membaca suhu analog |
| LCD 16x2 (dengan modul I2C) | 1 | Menampilkan data |
| DC Motor | 1 | Bertindak sebagai kipas |
| NPN Transistor (BJT) | 1 | Mengontrol daya motor DC via PWM |
| LED (Merah, Kuning, Hijau) | 3 | Indikator status |
| Push Button | 2 | Tombol Mode (C/F) & Tombol Darurat |
| Piezo Buzzer | 1 | Alarm audio |
| Resistor | Secukupnya | Pembatas arus untuk LED & pull-down/pull-up |
| Breadboard | 1 | Rangkaian |
| Kabel Jumper | Secukupnya | Rangkaian |

## Skema Rangkaian (Pinout)

Berikut adalah konfigurasi pin yang terhubung ke Arduino:

- **I2C LCD**: SDA  `A4`, SCL  `A5`
- **Sensor Suhu (TMP36)**: Vout  `A0`
- **LED Indikator**:
  - Hijau  `Pin 4`
  - Kuning  `Pin 5`
  - Merah  `Pin 6`
- **Piezo Buzzer**: Positif  `Pin 7`
- **Motor DC (via NPN Transistor)**: Basis Transistor  `Pin 9` (PWM)
- **Push Button**:
  - Button 1 (Ubah Mode C/F)  `Pin 8`
  - Button 2 (Darurat / Interrupt)  `Pin 2`

## Logika Operasional Sistem

Sistem beroperasi dengan parameter suhu berikut:

1. **Suhu < 25°C (Standby)**
   - Kipas: **Mati** (0 RPM)
   - Indikator: LED Merah menyala, Buzzer mati.
2. **Suhu 25°C - 29.9°C (Normal)**
   - Kipas: **Lambat** (PWM 100)
   - Indikator: LED Hijau menyala.
3. **Suhu 30°C - 35°C (Hangat)**
   - Kipas: **Cepat** (PWM 180)
   - Indikator: LED Kuning menyala.
4. **Suhu > 35°C (Overheat / Kritis)**
   - Kipas: **Maksimal** (PWM 255)
   - Indikator: LED Merah berkedip cepat & Buzzer berbunyi (Beep).

**Sistem Darurat (Interrupt):**
Jika tombol darurat ditekan kapan saja, pin 2 (Interrupt 0) akan tertrigger. Kipas, LED, dan buzzer akan langsung dimatikan, dan LCD akan mengunci tampilan dengan pesan **"SISTEM DARURAT DIMATIKAN"**. Board harus di-reset untuk mengembalikan fungsi normal.

## Cara Penggunaan

1. **Simulasi Tinkercad:**
   - Buat sirkuit baru di Tinkercad sesuai dengan tabel pinout di atas.
   - Salin seluruh kode yang ada di file `main.ino` (atau `src/main.cpp`).
   - *Paste* ke editor kode di Tinkercad.
   - Klik **Start Simulation**.
   - Klik sensor TMP36 untuk mengatur suhu simulasi secara manual dan lihat reaksi sistem.

2. **Implementasi Perangkat Keras (Hardware):**
   - Rangkai komponen sesuai skema. *Pastikan menggunakan dioda flyback pada motor DC untuk melindungi transistor.*
   - Install *library* tambahan di Arduino IDE: `LiquidCrystal I2C` oleh Frank de Brabander.
   - *Upload* kode ke papan Arduino Uno.

## Dependencies / Library

- `Wire.h` (Bawaan Arduino IDE untuk komunikasi I2C)
- `LiquidCrystal_I2C.h` (Untuk mengontrol LCD I2C)

---
*Dokumentasi diperbarui pada: 2026/04/27*
