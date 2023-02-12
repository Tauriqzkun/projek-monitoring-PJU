//sensor suhu PT100
void cekPt100(){
  thermo.begin(MAX31865_3WIRE);  // set to 2WIRE or 4WIRE as necessary
}
void ambildataPT100(){
  uint16_t rtd = thermo.readRTD();
  Serial.print("RTD value: "); Serial.println(rtd);
  float ratio = rtd;
  ratio /= 32768;
  Serial.print("Ratio = "); Serial.println(ratio, 8);
  Serial.print("Resistance = "); Serial.println(RREF * ratio, 8);
  Serial.print("Temperature = "); Serial.println(thermo.temperature(RNOMINAL, RREF));

  // Check and print any faults
  uint8_t fault = thermo.readFault();
  if (fault) {
    Serial.print("Fault 0x"); Serial.println(fault, HEX);
    if (fault & MAX31865_FAULT_HIGHTHRESH) {
      Serial.println("RTD High Threshold");
    }
    if (fault & MAX31865_FAULT_LOWTHRESH) {
      Serial.println("RTD Low Threshold");
    }
    if (fault & MAX31865_FAULT_REFINLOW) {
      Serial.println("REFIN- > 0.85 x Bias");
    }
    if (fault & MAX31865_FAULT_REFINHIGH) {
      Serial.println("REFIN- < 0.85 x Bias - FORCE- open");
    }
    if (fault & MAX31865_FAULT_RTDINLOW) {
      Serial.println("RTDIN- < 0.85 x Bias - FORCE- open");
    }
    if (fault & MAX31865_FAULT_OVUV) {
      Serial.println("Under/Over voltage");
    }
    thermo.clearFault();
  }
}
//SENSOR INA219
void cekina219() {
  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) {
      delay(10);
    }
  }
}
void ambildataINA219() {
  //ambil data asli
  float tegangan0 = ina219.getBusVoltage_V();
  if (isinf(tegangan0) || isnan(tegangan0))tegangan0 = 0;
  //rumus kalibrasi
  tegangan = 0.9933 * tegangan0 + 0.0221;

  float  arus0 = ina219.getCurrent_mA(); //data raw0  0 00
  if (isinf(arus0) || isnan(arus0))arus0 = 0;
  //rumus kalibrasi
  arus = 1.157 * arus0 - 90.157;
  if (arus < 0)arus = 0;

  power_mW = ina219.getPower_mW();
  if (isinf(power_mW) || isnan(power_mW))power_mW = 0;
  daya = tegangan * arus ;

  //kirim+tampilkan
  volt = tegangan;
  amp = arus;
  watt = daya;

}
//sensor DS18B20
void sensor_DS1() {
  Serial.print("Requesting temperatures...");
  sensor1.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  suhu1 = sensor1.getTempCByIndex(0);
  // kalibrasi1= sensor1.getTempCByIndex(0);
  //suhu1=kalibrasi1
  if (suhu1 == DEVICE_DISCONNECTED_C)
  {
    Serial.println("sensor mati 1");
    return;
  }
  Serial.print("Temp1 C: ");
  Serial.println(suhu1);
}
void sensor_DS2() {
  Serial.print("Requesting temperatures...");
  sensor2.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  suhu2 = sensor2.getTempCByIndex(0);
  // kalibrasi2= sensor2.getTempCByIndex(0);
  //suhu2=kalibrasi2
  if (suhu2 == DEVICE_DISCONNECTED_C)
  {
    Serial.println("sensor mati 2");
    return;
  }
  Serial.print("Temp2 C: ");
  Serial.println(suhu2);
}
void sensor_DS3() {
  Serial.print("Requesting temperatures...");
  sensor3.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  suhu3 = sensor3.getTempCByIndex(0);
  // kalibrasi3= sensor3.getTempCByIndex(0);
  //suhu3=kalibrasi3
  if (suhu3 == DEVICE_DISCONNECTED_C)
  {
    Serial.println("sensor mati 3");
    return;
  }
  Serial.print("Temp3 C: ");
  Serial.println(suhu3);
}
void sensor_DS4() {
  Serial.print("Requesting temperatures...");
  sensor4.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  suhu4 = sensor4.getTempCByIndex(0);
  // kalibrasi4= sensor4.getTempCByIndex(0);
  //suhu4=kalibrasi4
  if (suhu4 == DEVICE_DISCONNECTED_C)
  {
    Serial.println("sensor mati 4");
    return;
  }
  Serial.print("Temp4 C: ");
  Serial.println(suhu4);
}
//void sensor_DS5() {
//  Serial.print("Requesting temperatures...");
//  sensor5.requestTemperatures(); // Send the command to get temperatures
//  Serial.println("DONE");
//  suhu5 = sensor5.getTempCByIndex(0);
//  // kalibrasi5= sensor5.getTempCByIndex(0);
//  //suhu5=kalibrasi5
//  if (suhu5 == DEVICE_DISCONNECTED_C)
//  {
//    Serial.println("sensor mati 5");
//    return;
//  }
//  Serial.print("Temp5 C: ");
//  Serial.println(suhu5);
//}

//aktifkan sensor
void sensor1_aktif() {
  if (!sensor1.getAddress(insideThermometer, 0)) {
    Serial.println("tidak terhubung ke sensor1 DS18B20");
  }
  else {
    Serial.println("terhubung ke sensor1 DS18B20");
    sensor1.setResolution(insideThermometer, setresolusi_bit);
  }
  delay (250);
}
void sensor2_aktif() {
  if (!sensor2.getAddress(insideThermometer, 0)) {
    Serial.println("tidak terhubung ke sensor2 DS18B20");
  }
  else {
    Serial.println("terhubung ke sensor2 DS18B20");
    sensor2.setResolution(insideThermometer, setresolusi_bit);
  }
  delay (250);
}
void sensor3_aktif() {
  if (!sensor3.getAddress(insideThermometer, 0)) {
    Serial.println("tidak terhubung ke sensor3 DS18B20");
  }
  else {
    Serial.println("terhubung ke sensor3 DS18B20");
    sensor3.setResolution(insideThermometer, setresolusi_bit);
  }
  delay (250);
}
void sensor4_aktif() {
  if (!sensor4.getAddress(insideThermometer, 0)) {
    Serial.println("tidak terhubung ke sensor4 DS18B20");
  }
  else {
    Serial.println("terhubung ke sensor4 DS18B20");
    sensor4.setResolution(insideThermometer, setresolusi_bit);
  }
  delay (250);
}
//void sensor5_aktif() {
//  if (!sensor1.getAddress(insideThermometer, 0)) {
//    Serial.println("tidak terhubung ke sensor5 DS18B20");
//  }
//  else {
//    Serial.println("terhubung ke sensor5 DS18B20");
//    sensor5.setResolution(insideThermometer, setresolusi_bit);
//  }
//  delay (250);
//}
