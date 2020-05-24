

//#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"
#include "DHT.h"


//Variables
int chk;
float hum2; 
float temp2;
float hum3;
float temp3;
float hum4;
float temp4;

DHT dht2 (2, DHT11);
DHT dht3 (3, DHT11);
DHT dht4 (4, DHT11);

#define LOG_INTERVAL  10000 // mills between entries (reduce to take more/faster data)
#define SYNC_INTERVAL 10000 // mills between calls to flush() - to write data to the card

RTC_DS1307 RTC; // define the Real Time Clock object
// for the data logging shield, use digital pin 10 for the SD cs line
const int chipSelect = 10;

// the logging file
File logfile;

void error(char *str)
{
  Serial.print("error: ");
  Serial.println(str);

  while(1);
}

void setup(void)
{
  Serial.begin(9600);

  dht2.begin();
  dht3.begin();
  dht4.begin();
  RTC.begin();

   if (! RTC.isrunning())
  {
    Serial.println("RTC is NOT running!");
    //sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }


 // initialize the SD card
  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
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
  Serial.println("date, time, sensor, humidity %, temp");

  logfile.print("Logging to: ");
  logfile.println(filename);
  logfile.println("date, time, sensor, humidity %, temp");
  logfile.close();
}

void loop(void)
{
  
      //Read data and store it to variables hum and temp
    delay(2000);
    hum2 = dht2.readHumidity();
    temp2= dht2.readTemperature();
    hum3 = dht3.readHumidity();
    temp3= dht3.readTemperature();
    hum4 = dht4.readHumidity();
    temp4= dht4.readTemperature();


        //Print temp and humidity values to serial monitor
    DateTime now = RTC.now();
    Serial.print(String(now.year(), DEC) + '/' + String(now.month(), DEC) + '/' + String(now.day(), DEC));
    Serial.print(", ");
    Serial.print(String(now.hour(), DEC) + ':' + String(now.minute(), DEC) + ':' + String(now.second(), DEC));
    Serial.print(", ");
    Serial.print("Sensor 2,");
    Serial.print(hum2);
    Serial.print(",");
    Serial.println(temp2);
    

    Serial.print(String(now.year(), DEC) + '/' + String(now.month(), DEC) + '/' + String(now.day(), DEC));
    Serial.print(", ");
    Serial.print(String(now.hour(), DEC) + ':' + String(now.minute(), DEC) + ':' + String(now.second(), DEC));
    Serial.print(", ");
    Serial.print("Sensor 3,");
    Serial.print(hum3);
    Serial.print(",");
    Serial.println(temp3);
   

    Serial.print(String(now.year(), DEC) + '/' + String(now.month(), DEC) + '/' + String(now.day(), DEC));
    Serial.print(", ");
    Serial.print(String(now.hour(), DEC) + ':' + String(now.minute(), DEC) + ':' + String(now.second(), DEC));
    Serial.print(", ");
    Serial.print("Sensor 4,");
    Serial.print(hum4);
    Serial.print(", ");
    Serial.println(temp4);
  
    delay(2000); //Delay 2 sec.

    //write to SD card
    //logfile = SD.open(filename, FILE_WRITE);

    logfile.print(String(now.year(), DEC) + '/' + String(now.month(), DEC) + '/' + String(now.day(), DEC));
    logfile.print(", ");
    logfile.print(String(now.hour(), DEC) + ':' + String(now.minute(), DEC) + ':' + String(now.second(), DEC));
    logfile.print(", ");
    logfile.print("Sensor 2,");
    logfile.print(hum2);
    logfile.print(",");
    logfile.println(temp2);

    logfile.print(String(now.year(), DEC) + '/' + String(now.month(), DEC) + '/' + String(now.day(), DEC));
    logfile.print(", ");
    logfile.print(String(now.hour(), DEC) + ':' + String(now.minute(), DEC) + ':' + String(now.second(), DEC));
    logfile.print(", ");
    logfile.print("Sensor 3,");
    logfile.print(hum3);
    logfile.print(",");
    logfile.println(temp3);


    logfile.print(String(now.year(), DEC) + '/' + String(now.month(), DEC) + '/' + String(now.day(), DEC));
    logfile.print(", ");
    logfile.print(String(now.hour(), DEC) + ':' + String(now.minute(), DEC) + ':' + String(now.second(), DEC));
    logfile.print(", ");
    logfile.print("Sensor 4,");
    logfile.print(hum4);
    logfile.print(", ");
    logfile.println(temp4);
 //   logfile.close();
    delay(2000); //Delay 2 sec.
    
}
