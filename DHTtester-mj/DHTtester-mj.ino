// ESP8266 or Arduinio reading multiple DHT (11,21,22) Temperature and Humidity Sensors
// (c) D L Bird 2016

#include "DHT.h"   //https://github.com/adafruit/DHT-sensor-library 
#include "RTClib.h"
#include <SPI.h>
#include <SD.h>
#include <Wire.h>

#define DHTTYPE DHT11   // DHT 11
#define LOG_INTERVAL  10000 // mills between entries (reduce to take more/faster data)
#define SYNC_INTERVAL 10000 // mills between calls to flush() - to write data to the card
uint32_t syncTime = 0; // time of last sync()
#define ECHO_TO_SERIAL   1 // echo data to serial port
#define WAIT_TO_START    0 // Wait for serial input in setup()

RTC_DS1307 rtc;
const int chipSelect = 10;
File logfile;

// Create the DHT temperature and humidity sensor object
DHT dht2(2, DHTTYPE);
DHT dht3(3, DHTTYPE);
DHT dht4(4, DHTTYPE);

void setup(void) {
  Serial.begin(115200);
  dht2.begin();
  dht3.begin();
  dht4.begin();
  rtc.begin();


    // initialize the SD card
  Serial.print("Initializing SD card...");



// create a new file
  char filename[] = "LOGGER00.CSV";
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = i/10 + '0';
    filename[7] = i%10 + '0';
    if (! SD.exists(filename)) {
      // only open a new file if it doesn't exist
      logfile = SD.open(filename, FILE_WRITE);
      break;  // leave the loop!
    }
  }
 

 
  Serial.print("Logging to: ");
  Serial.println(filename);

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    }
    //rtc.adjust(DateTime(2020, 4, 1, 3, 34, 0));
  
}

void loop() {
  // Read DHT temperature and humidity values
  float D2_t = dht2.readTemperature();
  float D2_h = dht2.readHumidity();

  float D3_t = dht3.readTemperature();
  float D3_h = dht3.readHumidity();

  float D4_t = dht4.readTemperature();
  float D4_h = dht4.readHumidity();

  DateTime now = rtc.now();


    logfile.print(now.year(), DEC);
    logfile.print('/');
    logfile.print(now.month(), DEC);
    logfile.print('/');
    logfile.print(now.day(), DEC);
    logfile.print(",");


// write to serial
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(",");
    //Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    //Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
//   Serial.print();

 
 //Full Timestamp Serial.println(String("DateTime::TIMESTAMP_FULL:\t")+time.timestamp(DateTime::TIMESTAMP_FULL));
  
  Serial.print(",D2,");
  Serial.print(D2_t,1); Serial.print("C,");
  Serial.print(D2_h,1); Serial.print("%");

  Serial.print(",D3,");
  Serial.print(D3_t,1); Serial.print("C,");
  Serial.print(D3_h,1); Serial.print("%,");

  Serial.print(",D4,");
  Serial.print(D4_t,1); Serial.print("C,");
  Serial.print(D4_h,1); Serial.println("%");

  delay(3000);

if ((millis() - syncTime) < SYNC_INTERVAL) return;
  syncTime = millis();

}
