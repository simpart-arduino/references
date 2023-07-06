#include <DallasTemperature.h>
#include <OneWire.h>
#include <SPI.h> 
#include <SD.h>
#include <DS3232RTC.h>
#include <string.h>

#define ONE_WIRE_BUS  9
#define SENSER_BIT    9

const int chipSelect = 10;
File dataFile;
tmElements_t tm;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);


void setup() {
  if (!SD.begin(chipSelect)) {
    return;
  }
}

void loop() {
  /* get time */
  RTC.read(tm);
  String tmstr = String(tm.Year + 1970) + "/" + String(tm.Month) + "/" + String(tm.Day) + " " + String(tm.Hour) + ":" + String(tm.Minute) + ":" + String(tm.Second);

  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);
  /* write to sd */
  dataFile = SD.open("datalog.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.println(tmstr + " " + String(temp));
    //dataFile.println(tmstr);
  }
  
  dataFile.close();
  //delay(500);
}
