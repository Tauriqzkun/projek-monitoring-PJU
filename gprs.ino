void cekGPRS() {
  for (byte i = 1; i < 4; i++) {
    Serial.print(F("\r\nCEK "));
    Serial.println(i);

    if (gprsComm() == 0) {
      Serial.println(F("\r\n- GPRS ERROR -"));
      delay(1000);
      if (i == 3) {
        karakter = 'E';
      }
    }
    else {
      Serial.println(F("\r\nKomunikasi GPRS OK.")); // Kita Lanjut ke step berikutnya
      gprsShut(500);
      break;
    }
  }
}

boolean gprsComm() {
  //CHECK GPRS SERVICE REGISTRATION
  Serial.println(F("\r\n- CEK REG GPRS -"));

  // Cek GPRS Registration
  char karakterOutput = '.';
  gprsReg(karakterOutput, 10000);
  if (karakterOutput != '1') return 0;

  Serial.flush();
  Serial1.flush();
  Serial.println(F("\r\n- TUTUP TCP IP -"));
  gprsShut(2000);

  // SET SINGLE IP CONNECTION
  Serial.println(F("\r\n\r\n- ATUR CIPMUX -")); //MENJADI SINGLE IP CONNECTION
  gprsMux(900);

  //Attach from GPRS Service
  Serial.println(F("\r\n\r\n- AKTIFKAN GPRS -")); //SERVICE
  gprsAttach(900);

  //CONNECTION TYPE = gprs ; ATUR APN, USER, DAN PASSWORD
  Serial.println(F("\r\n\r\n- ISI APN -")); //, USER, DAN PASSWORD
  gprsCSTT(500);

  //CEK IP STATUS IN INITIAL MODE
  Serial.print(F("\r\n\r\n- CEK IP STATUS-\r\n"));
  gprsStatus(900);

  //Bring Up Wireless Connection with GPRS
  Serial.println(F("\r\n\r\n- BUKA TCP IP -"));
  karakterOutput = '.';
  gprsWirelessConnect(karakterOutput, 85000);
  if ( karakterOutput == 'E') {
    Serial.println(F("- ERROR -"));
    return 0;
  }
  Serial.println(F("- CONNECTED -"));

  //GPRS IP
  Serial.println(F("\r\n- IP ADDRESS -"));
  gprsIP(1000);

  return 1;
}

//---------------------------------------
// gprs SOURCE CODE ROOT
//---------------------------------------
char cekSerial(unsigned long times) {
  boolean indeks = 0;
  char karakter = '\0';
  unsigned long mulai = millis();
  while (millis() - mulai < times && indeks == 0) {
    while (serial2.available() > 0) {
      karakter = serial2.read();
      if (isAscii(karakter)) Serial.write(karakter);

      if (karakter == 'O' || karakter == 'E') {
        indeks = 1;
        break;
      } // exit from if
    } // exit from serial2 available
  } // exit from millis()

  readSerial(100);
  return karakter;
}

// ATTACH gprs SERVICE
void gprsReg(char &karakter, unsigned int jeda) {
  unsigned long waktu = millis ();
  while (millis () - waktu <= jeda) {
    char g = '+';
    serial2.println(F("AT+CGREG?"));
    unsigned long waktuAwal = millis();
    while (millis() - waktuAwal <= 300 && g != 'K') {
      g = serial2.read();
      switch (g) {
        case '0' ... '9':
          karakter = g;

          if (isAscii(karakter)) Serial.print(karakter);

          break;

        default:

          if (isAscii(g)) Serial.print(g);

          break;
      }
    }


    Serial.print(F("\r\nCGREG="));
    Serial.println(karakter);


    if (karakter != '1') {
      serial2.println(F("AT+CGREG=1"));
      readSerial(200);
    }
    else break;
  }

}

void gprsAttach(unsigned int waktu) {
  // ATTACH gprs SERVICE
  serial2.println(F("AT+CGATT=1"));
  readSerial(waktu);
}

void gprsStatus(unsigned int waktu) {
  // ATTACH gprs SERVICE
  byte i = 0;
  unsigned long mulai = millis();
  serial2.println(F("AT+CIPSTATUS"));
  while (millis() - mulai <= waktu && i < 4) {
    while (serial2.available() > 0) {
      char g = serial2.read();
      Serial.write(g);
      if (g == '\n') i = i + 1;
    }
  }
  readSerial(20);
}

void gprsShut(unsigned int waktu) {
  //DEAKTIVASI gprsPDP CONTEXT DAN TUTUP BEARER
  serial2.println(F("AT+CIPSHUT"));
  readSerial(waktu);
}

void gprsMux(unsigned int waktu) {
  // ATTACH gprs SERVICE
  serial2.println(F("AT+CIPMUX=0"));
  readSerial(waktu);
}

void gprsCSTT(unsigned int waktu) {
  byte indeks;
  //APN
  serial2.print(F("AT+CSTT=\""));
  serial2.print(apn);

  //USER - ISI DI ANTARA \" DAN \"
  serial2.print(F("\",\"\","));

  //PASSWORD - ISI DI ANTARA \" DAN \"
  serial2.println(F("\"\""));
  readSerial(waktu);
}

void gprsWirelessConnect(char &karakter, unsigned long waktu) {
  serial2.println(F("AT+CIICR"));
  char g = cekSerial(waktu);

  Serial.print(F("\r\nKarakter dari CIICR : "));
  Serial.print(g);

  if (g == 'O')  Serial.println(F("K"));
  else {
    Serial.println(F("RROR"));
  }
}

void gprsIP(unsigned int waktu) {
  serial2.println(F("AT+CIFSR"));
  readSerial(waktu);
}

// LOW CODE FOR TCP IP
// --------------------------------------------------
// FUNCTION FOR TCP IP
boolean TCPstart(unsigned long jeda, byte ulangan) {
  char g;
  boolean hasilTCP = 0;
  for (byte angka = 1; angka <= ulangan; angka++) {
    Serial.print(F(" - TCP ke - "));
    Serial.println(angka);
    Serial.flush();
    Serial1.flush();

    serial2.print("AT+CIPSTART=");
    serial2.print("\"TCP\",\"");
    serial2.print(HTTPSERVER);
    readSerial(200);
    serial2.print(F("\",\""));
    serial2.print(HTTPPORT);
    serial2.println("\"");

    unsigned long waktu = millis();
    while (waktu + jeda > millis() ) {
      g = serial2.read();
      if (isAscii(g))  Serial.print(g);
      if (g == 'S' || g == 'C' || g == 'A' || g == 'E') break;
    }

    readSerial(150);

    if (g == 'C' || g == 'A') {
      hasilTCP = 1;
      break;
    }
    if (g == 'E' || g == 'S') {
      hasilTCP = 0;
      break;
    }
  }

  Serial.print(F("\r\nHasil TCP Start="));
  Serial.println(hasilTCP);


  if (hasilTCP == 0) readSerial(2000);
  return hasilTCP;
}

boolean TCPsend() {
  //siapkan variabel kode HTTP
  kode = 0;
  char karakter = '.';
  serial2.println(F("AT+CIPSEND"));
  unsigned long waktu = millis();
  while (millis() - waktu < 2000 && karakter != '>') {
    while (serial2.available() > 0 && karakter != '>') {
      karakter = serial2.read();
      Serial.write(karakter);
    }
  }
  if (karakter != '>') return 0;
  delay(10);
  serial2.print("POST /api/v1.6/devices/");
  serial2.print(DEVICE_LABEL);
  serial2.print(F("/ HTTP/1.1\r\n"));
  serial2.print(F("Host: "));
  serial2.print(HTTPSERVER);
  serial2.print(F("\r\n"));
  readSerial(500);

  serial2.print(F("User-Agent: "));
  serial2.print(USER);
  serial2.print(F("/"));
  serial2.print(VERSION);
  serial2.print(F("\r\n"));
  serial2.print(F("X-Auth-Token: "));
  serial2.print(TOKEN);
  serial2.print(F("\r\n"));
  //  serial2.print(F("Connection: close\r\n"));
  readSerial(500);

  serial2.print(F("Content-Type: application/json\r\n"));
  serial2.print(F("Content-Length: "));
  serial2.print(json.length());
  serial2.print(F("\r\n\r\n"));
  readSerial(100);

  serial2.print(json);
  serial2.print(F("\r\n"));
  readSerial(50);
  Serial.write(13);
  Serial.write(10);

  char character = '\0';
  unsigned long waktuAwal = millis();
  // SEND OK
  waktuAwal = millis();
  boolean nilai = 0;
  serial2.println((char)26);
  while (millis() - waktuAwal < 15000) {
    character = serial2.read();
    if (isAscii(character)) Serial.write(character);
    if (character == 'H') break;
  }
  readSerial(1000);
}

void TCPclose(unsigned int waktu) {
  serial2.println(F("AT+CIPCLOSE"));
  readSerial(waktu);
}
