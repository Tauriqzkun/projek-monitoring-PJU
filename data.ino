void ambilWaktu() {
  //AMBIL WAKTU
  waktu = rtc.now();
}

void dataJSON() {
  json = "";
  json = "{\"suhu1\":" + String(suhu1, 2);
  json += ",\"suhu2\":" + String(suhu2, 2);
  json += ",\"suhu3\":" + String(suhu3, 2);
  json += ",\"suhu4\":" + String(suhu4, 2);
  json += ",\"suhu5\":" + String(thermo.temperature(RNOMINAL, RREF),2);
  json += ",\"volt\":" + String(volt, 2);
  json += ",\"amp\":" + String(amp, 2);
  json += ",\"watt\":" + String(watt, 2);
  json += "}";
  Serial.println(json);
}

void kirimData() {
  boolean nilai = TCPstart(5000, 1);
  dataJSON();
  //SET HTTP PARAMETERS VALUE
  Serial.flush();
  Serial1.flush();
  Serial.println(F("\r\n - KIRIM DATA - "));
  nilai = TCPsend();
  Serial.println(F("\r\n"));
  TCPclose(500);
  readSerial(500);
  Serial.flush();
  Serial1.flush();
}
