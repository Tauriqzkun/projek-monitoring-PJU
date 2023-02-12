//pin sensor ds18b20
#define ONE_WIRE_BUS1 25
#define ONE_WIRE_BUS2 26
#define ONE_WIRE_BUS3 27
#define ONE_WIRE_BUS4 14
//#define ONE_WIRE_BUS5 15
#define setresolusi_bit 12
#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP (interval * 60)    /* Time ESP32 will go to sleep (in seconds) */
#define CLOCK_INTERRUPT_PIN 15 //PIN SQW
#define LED 12
#include <HardwareSerial.h>
#define s2
//tidur
byte detik;
byte interval = 1; // Menit // interval pengirimam

static const byte RXPin2 = 16, TXPin2 = 17;
HardwareSerial serial2(2);
#ifdef s2
#define SERIAL1 serial2
#endif

//SD CARD
String filename;
const int chipSelect = 5; //sesuai dengan yang di ESP32
const int cs = 4;//pin untuk max3165
unsigned int indeks = 1;

//variabel
unsigned int kode;
boolean koneksi = 0;
boolean awal = 1;
boolean kirim = 0;
String operators, network;
char karakter;
byte a, b;
unsigned int nilai, i, j;

//sensor suhu
float suhu1, suhu2, suhu3, suhu4, suhu5;
float kalibrasi1,kalibrasi3,kalibras3,kalibrasi4,kalibrasi5;
//ina219
float volt,amp,watt;

float tegangan, arus, daya, power_mW;
float tegangan_1, arus_1, daya_1, power_mW_1;
//EEPROM
RTC_DATA_ATTR unsigned int nomor = 0;
RTC_DATA_ATTR char namaFile[14] = "/LOG0001.txt";
