#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inisiasi nomor pin
const int pinSuhu      = A0;  
const int pinBtnMode   = 8;   // Tombol Switch C/F
const int pinBtnEmg    = 2;   // Tombol Emergency
const int pinLedHijau  = 4;   
const int pinLedKuning = 5;   
const int pinLedMerah  = 6;   
const int pinBuzzer    = 7;   
const int pinKipas     = 9;   

// konfigurasi lcd dan variabel
LiquidCrystal_I2C lcd(0x27, 16, 2);

volatile bool statusDarurat = false;
bool modeFahrenheit = false; 
bool statusTombolLama = HIGH; 

void setup() {
  pinMode(pinSuhu, INPUT);
  pinMode(pinBtnMode, INPUT_PULLUP); 
  pinMode(pinBtnEmg, INPUT_PULLUP);
  
  pinMode(pinLedHijau, OUTPUT);
  pinMode(pinLedKuning, OUTPUT);
  pinMode(pinLedMerah, OUTPUT);
  pinMode(pinBuzzer, OUTPUT);
  pinMode(pinKipas, OUTPUT);

  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Suhu : ");
  lcd.setCursor(0, 1);
  lcd.print("Kipas: ");

  attachInterrupt(digitalPinToInterrupt(pinBtnEmg), daruratTerjadi, FALLING);
}

void loop() {
  if (statusDarurat) {
    jalankanProsedurDarurat();
    return; 
  }

  // Logika switch button mode (C/F)
  bool statusTombolSekarang = digitalRead(pinBtnMode);
  if (statusTombolSekarang == LOW && statusTombolLama == HIGH) {
    modeFahrenheit = !modeFahrenheit; 
    delay(50); 
  }
  statusTombolLama = statusTombolSekarang;

  // Sensor read
  int nilaiADC   = analogRead(pinSuhu);
  float tegangan = nilaiADC * 5.0 / 1024.0;
  float suhuC    = (tegangan - 0.5) * 100;

  float suhuTampil = modeFahrenheit ? (suhuC * 9.0 / 5.0) + 32.0 : suhuC;
  char unit = modeFahrenheit ? 'F' : 'C';

  String statusKipas = "";
  int kecepatanKipas = 0;

  // Logika pengontrolan kecepatan menggunakan PWM
  if (suhuC < 25) {
    statusKipas = "Mati    ";
    kecepatanKipas = 0;
    digitalWrite(pinLedMerah, HIGH); // Lampu Merah standby saat mati
    digitalWrite(pinLedHijau, LOW);
    digitalWrite(pinLedKuning, LOW);
    noTone(pinBuzzer);
  } 
  else if (suhuC >= 25 && suhuC < 30) {
    statusKipas = "Lambat  ";
    kecepatanKipas = 100; // Kecepatan Rendah
    digitalWrite(pinLedMerah, LOW);
    digitalWrite(pinLedHijau, HIGH); // Lampu Hijau saat lambat
    digitalWrite(pinLedKuning, LOW);
    noTone(pinBuzzer);
  } 
  else if (suhuC >= 30 && suhuC <= 35) {
    statusKipas = "Cepat   ";
    kecepatanKipas = 180; // Kecepatan Menengah
    digitalWrite(pinLedMerah, LOW);
    digitalWrite(pinLedHijau, LOW);
    digitalWrite(pinLedKuning, HIGH); // Lampu Kuning saat cepat
    noTone(pinBuzzer);
  } 
  else { 
    statusKipas = "Maksimal";
    kecepatanKipas = 255; // Kecepatan Penuh
    digitalWrite(pinLedHijau, LOW);
    digitalWrite(pinLedKuning, LOW);
    
    // Indikator Merah Berkedip & Buzzer
    digitalWrite(pinLedMerah, HIGH);
    tone(pinBuzzer, 1000);
    delay(100);
    digitalWrite(pinLedMerah, LOW);
    noTone(pinBuzzer);
    delay(100);
  }

  analogWrite(pinKipas, kecepatanKipas);

  // output LCD
  lcd.setCursor(7, 0);
  lcd.print(suhuTampil);
  lcd.print(" ");
  lcd.print(unit);
  lcd.print("    "); 
  
  lcd.setCursor(7, 1);
  lcd.print(statusKipas);

  delay(100); 
}

void daruratTerjadi() {
  statusDarurat = true;
}

void jalankanProsedurDarurat() {
  analogWrite(pinKipas, 0);
  digitalWrite(pinLedHijau, LOW);
  digitalWrite(pinLedKuning, LOW);
  digitalWrite(pinLedMerah, LOW);
  noTone(pinBuzzer);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SISTEM DARURAT ");
  lcd.setCursor(0, 1);
  lcd.print("   DIMATIKAN   ");
  while(true); 
}
