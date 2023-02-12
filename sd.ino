
void cekRTC() {
  if (!rtc.begin()) {
    Serial.println("RTC Tidak Terbaca");
    digitalWrite(LED, HIGH); // LED RTC ERROR
    Serial.flush();
    while (1);
  }
  Serial.println("RTC Terbaca");
}
void ceksd() {

  if (!SD.begin(chipSelect)) {
    Serial.println("Micro SD Tidak Terbaca");
    digitalWrite(LED, HIGH); // LED SDCARD ERROR
    while (1);
  }
  Serial.println("Micro SD Terbaca");

}
void cekfile() {
  Serial.print(" cek file = ");
  boolean ada = 1;
  while (ada == 1) {
    sprintf (namaFile, "/LOG%04d.txt", nomor);
    ada = SD.exists (namaFile);
    if (ada == 1) nomor ++;
    if (ada == 0) {
      //Membuat File
      Serial.println(namaFile);
      File dataFile = SD.open(namaFile, FILE_WRITE);
      dataFile.println ("Tanggal (yyyy/mm/dd hh:mm:ss), suhu1,suhu2,suhu3,suhu4,suhu5,volt,amp,watt, Dates (yyyy/mm/dd hh:mm:ss)");
      dataFile.close();
    }
  }
}
void simpanData() {
  filename = String(namaFile);
  //  Serial.println(namaFile);
  //  Serial.println(filename);
  File dataFile = SD.open(filename, FILE_APPEND);
  if (dataFile) {
    dataFile.print(waktu.timestamp(DateTime::TIMESTAMP_DATE));//TAHUN, BULAN, HARI
    dataFile.print(" ");
    dataFile.print(waktu.timestamp(DateTime::TIMESTAMP_TIME));//JAM, MENIT, DETIK
    dataFile.print(","); dataFile.print(suhu1); //
    dataFile.print(","); dataFile.print(suhu2); //
    dataFile.print(","); dataFile.print(suhu3); //
    dataFile.print(","); dataFile.print(suhu4); //
    dataFile.print(","); dataFile.print(thermo.temperature(RNOMINAL, RREF)); // PT100
    dataFile.print(","); dataFile.print(volt); //
    dataFile.print(","); dataFile.print(amp); //
    dataFile.print(","); dataFile.print(watt); //
    
    dataFile.close();
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
  }
  else {
    Serial.println("error opening datalog.txt");
    digitalWrite(LED, HIGH); // LED SDCARD ERROR
  }
}
