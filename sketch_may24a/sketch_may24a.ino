#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"

// A simple data logger for the Arduino analog pins
#define DS1307_ADDRESS 0x68
// how many milliseconds between grabbing data and logging it. 1000 ms is once a second
#define LOG_INTERVAL  10000 // mills between entries (reduce to take more/faster data)

// how many milliseconds before writing the logged data permanently to disk
// set it to the LOG_INTERVAL to write each time (safest)
// set it to 10*LOG_INTERVAL to write all data every 10 datareads, you could lose up to
// the last 10 reads if power is lost but it uses less power and is much faster!
#define SYNC_INTERVAL 10000 // mills between calls to flush() - to write data to the card
uint32_t syncTime = 0; // time of last sync()

#define ECHO_TO_SERIAL   1 // echo data to serial port
#define WAIT_TO_START    0 // Wait for serial input in setup()

// The analog pins that connect to the sensors
#define Voltage A1                // analog 1

RTC_DS1307 RTC; // define the Real Time Clock object

// for the data logging shield, we use digital pin 10 for the SD cs line
const int chipSelect = 10;

// the logging file
File logfile;
char filename;

void error(char *str)
{
  Serial.print("error: ");
  Serial.println(str);
 


  while(1);
}

void setup(void)
{
  Serial.begin(9600);
  Serial.println();
 
 
#if WAIT_TO_START
  Serial.println("Type any character to start");
  while (!Serial.available());
#endif //WAIT_TO_START

  // initialize the SD card
  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
 
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    error("Card failed, or not present");
  }
  Serial.println("card initialized.");
 
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
 
  if (! logfile) {
    error("couldnt create file");
  }
 
  Serial.print("Logging to: ");
  Serial.println(filename);

  logfile.print("Logging to: ");
  logfile.println(filename);

  logfile.println("millis,stamp,datetime,voltage");   
  Serial.println("millis,stamp,datetime,voltage");
  logfile.close();//>>>>>>>>>>>>>>>>>>>>>>CLOSE
}

void loop(void)
{
  DateTime now;

  // delay for the amount of time we want between readings
  delay((LOG_INTERVAL -1) - (millis() % LOG_INTERVAL));
 
 


  // fetch the time
  now = RTC.now();
  // log time
  logfile = SD.open(filename, FILE_WRITE);
 logfile.print(now.unixtime()); // seconds since 1/1/1970
  logfile.print(", ");
  logfile.print('"');
  logfile.print(now.year(), DEC);
  logfile.print("/");
  logfile.print(now.month(), DEC);
  logfile.print("/");
  logfile.print(now.day(), DEC);
  logfile.print(" ");
  logfile.print(now.hour(), DEC);
  logfile.print(":");
  logfile.print(now.minute(), DEC);
  logfile.print(":");
  logfile.print(now.second(), DEC);
  logfile.print('"');
#if ECHO_TO_SERIAL
  Serial.print(now.unixtime()); // seconds since 1/1/1970
  Serial.print(", ");
  Serial.print('"');
  Serial.print(now.year(), DEC);
  Serial.print("/");
  Serial.print(now.month(), DEC);
  Serial.print("/");
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(":");
  Serial.print(now.minute(), DEC);
  Serial.print(":");
  Serial.print(now.second(), DEC);
  Serial.print('"');
#endif //ECHO_TO_SERIAL

 
  analogRead(Voltage);
  delay(10);
  int tempReading = analogRead(Voltage);   
 
  // converting that reading to voltage, for 3.3v arduino use 3.3, for 5.0, use 5.0
//  float voltage = tempReading * aref_voltage / 1024; 
  float voltage = tempReading * (5.0 / 1023.0);
 
 
  logfile.print(", ");   
  logfile.print(voltage);
#if ECHO_TO_SERIAL

  Serial.print(", ");   
  Serial.print(voltage);
#endif //ECHO_TO_SERIAL

 
  // Now we write data to disk! Don't sync too often - requires 2048 bytes of I/O to SD card
  // which uses a bunch of power and takes time
  if ((millis() - syncTime) < SYNC_INTERVAL) return;
  syncTime = millis();

  logfile.close();//>>>>>>>>>>>>>>>>>>>>>>CLOSE
   
}
