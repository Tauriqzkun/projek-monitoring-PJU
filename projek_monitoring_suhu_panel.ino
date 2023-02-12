#include "variabel.h"//config gsm+ubidots
#include "config.h" // simpan variabel
//sd card
#include <SPI.h>
#include <SD.h>
//#include <SdFat.h>
File file;
//INA219
#include <Adafruit_INA219.h>
Adafruit_INA219 ina219;
//rtc
#include "RTClib.h"
RTC_DS3231 rtc;
DateTime waktu, waktunanti;
//ds18b20
#include <OneWire.h>
#include <DallasTemperature.h>
OneWire oneWire1(ONE_WIRE_BUS1); //pin 25
OneWire oneWire2(ONE_WIRE_BUS2); //pin 26
OneWire oneWire3(ONE_WIRE_BUS3); //pin 33
OneWire oneWire4(ONE_WIRE_BUS4); //pin 14
//OneWire oneWire5(ONE_WIRE_BUS5); //pin 15
DallasTemperature sensor1(&oneWire1);
DallasTemperature sensor2(&oneWire2);
DallasTemperature sensor3(&oneWire3);
DallasTemperature sensor4(&oneWire4);
//DallasTemperature sensor5(&oneWire5);
DeviceAddress insideThermometer;

#include <Adafruit_MAX31865.h>
Adafruit_MAX31865 thermo = Adafruit_MAX31865(cs);
// The value of the Rref resistor. Use 430.0 for PT100 and 4300.0 for PT1000
#define RREF      431.0
// The 'nominal' 0-degrees-C resistance of the sensor
// 100.0 for PT100, 1000.0 for PT1000
#define RNOMINAL  100.0

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  delay(1000);
  Serial.println("\r\ntest multiserial ESP32");
#ifdef s2
  SERIAL1.begin(9600, SERIAL_8N1, RXPin2, TXPin2);
  Serial.println("serial2");
#endif

  pinMode(LED, OUTPUT);
  pinMode(chipSelect, OUTPUT);
  Serial.println ("Alat Monitoring Suhu Panel Surya");

  //INIT SENSOR SUHU DS18B20
  Serial.print("Initializing DS18B20...");
  sensor1_aktif();
  sensor2_aktif();
  sensor3_aktif();
  sensor4_aktif();
  //sensor5_aktif();
  //INIT PT100
  Serial.println("Initializing PT-100...");
  cekPt100();
  //INIT INA219
  Serial.println("Initializing INA219...");
  cekina219();
  //INIT RTC
  Serial.println("Initializing RTC...");
  cekRTC();
  //  //INIT MICRO SD
  Serial.println("Initializing SD card...");
  //ceksd();

  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  if (wakeup_reason == 2) {
    loop();
  }
  digitalWrite(LED, LOW);

  //cekfile
 // cekfile();
  //persiapan gsm
  for (byte i = 0; i < 7; i++) {
    Serial.println(6 - i);
    digitalWrite(LED, HIGH);
    delay(1000);
    digitalWrite(LED, LOW);
    delay(1000);
  }


  //INIT GSM
  Serial.println("Initialization GSM");
  delay(2000);

  Serial.println("\n> Check GSM");
  for (i = 1; i < 5; i++) {
    Serial.print(i);
    Serial.print(" ");
    j = ConnectAT(200);
    if (j == 8) break;
  }
  if ( j == 6) {
    Serial.println("GSM Error!!!");
    while (1);
  }

  Serial.println("GSM OK!");
  delay(2000);

  Serial.println("GSM Ready to Use!");
  delay(2000);
  initGSM();

  //operator
  Serial.println("Check Operator");
  gsmOperator();
  delay(2000);


  //kualitas sinyal
  Serial.println("Check Signal Quality");
  gsmSinyal();
  delay(2000);

  Serial.println("Check Internet Connection");
  //Cek GPRS
  cekGPRS();
  Serial.println("done");
  delay(2000);

  //HTTP TERMINATE
  Serial.flush();
  Serial1.flush();
  Serial.println(F("\r\n - TUTUP TCP IP - "));
  TCPclose(500);
  koneksi = 1;
  Serial.flush();
  Serial1.flush();

  //tunggu hingga detiknya nol
  detik = 50;
  while (detik != 0) {
    delay(200);
    ambilWaktu();
    detik = waktu.second();
    Serial.println(detik);
  }
}


void loop() {
  Serial.println(F("\r\n - ambil data - "));
  ambilWaktu();
  Serial.print(waktu.timestamp(DateTime::TIMESTAMP_DATE));//TAHUN, BULAN, HARI
  Serial.print(" ");
  Serial.println(waktu.timestamp(DateTime::TIMESTAMP_TIME));//JAM, MENIT, DETIK


  //ambilSuhu();
  sensor_DS1();
  sensor_DS2();
  sensor_DS3();
  sensor_DS4();
  //sensor_DS5();
  ambildataPT100();
  //ambil ina219
  ambildataINA219();
  //simpanData();
  Serial.flush();
  Serial1.flush();
  
  gprsComm();
  kirimData();

  //simpan waktu setelah data berhasil diterima oleh server
  filename = String(namaFile);
  Serial.println(filename);
  DateTime dates = rtc.now();
  File dataFile = SD.open(filename, FILE_APPEND);
  dataFile.print(",");
  dataFile.print(dates.timestamp(DateTime::TIMESTAMP_DATE));//TAHUN, BULAN, HARI
  dataFile.print(" ");
  dataFile.println(dates.timestamp(DateTime::TIMESTAMP_TIME));//JAM, MENIT, DETIK
  dataFile.close();

  //MENCETAK KE SERIAL MONITOR
  Serial.print(waktu.timestamp(DateTime::TIMESTAMP_DATE));//TAHUN, BULAN, HARI
  Serial.print(" ");
  Serial.print(waktu.timestamp(DateTime::TIMESTAMP_TIME));//JAM, MENIT, DETIK
  //  Serial.print(" , ");
  //  Serial.print (jarak);
  //  Serial.print (" , ");
  //  Serial.print (jarakbaru);
  //  Serial.print (" , ");
  //  Serial.print(kedalaman);
  //  Serial.print (" , ");

  // atur dulu alaram RTC nya
  aturAlarm();
  sleepmode();
}

void onAlarm() {
  Serial.println("Alarm occured!");
}

void aturAlarm() {
  rtc.disable32K();

  pinMode(CLOCK_INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(CLOCK_INTERRUPT_PIN), onAlarm, FALLING);

  // set alarm 1, 2 flag to false (so alarm 1, 2 didn't happen so far)
  // if not done, this easily leads to problems, as both register aren't reset on reboot/recompile
  rtc.clearAlarm(1);
  rtc.clearAlarm(2);

  // stop oscillating signals at SQW Pin
  // otherwise setAlarm1 will fail
  rtc.writeSqwPinMode(DS3231_OFF);

  // turn off alarm 2 (in case it isn't off already)
  // again, this isn't done at reboot, so a previously set alarm could easily go overlooked
  rtc.disableAlarm(2);

  //bangun setiap interval dalam menit
  waktunanti = waktu + interval * 60;
  waktu = DateTime(waktunanti.year(), waktunanti.month(), waktunanti.day(), waktunanti.hour(), waktunanti.minute(), 0);
  rtc.setAlarm1(
    waktu,
    DS3231_A1_Hour // this mode triggers the alarm when the seconds match. See Doxygen for other options
  );
}

void sleepmode() {
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_4, 0);
  //ATUR SLEEP TIME
  //  Serial.println("tidur");
  Serial.flush();
  esp_deep_sleep_start();
}
