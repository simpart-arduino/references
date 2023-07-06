#include <SPI.h> 
#include <SD.h>
#include <DS3232RTC.h>
#include <string.h>

const int chipSelect = 10;
File dataFile;
tmElements_t tm;


void setup() {
  if (!SD.begin(chipSelect)) {
    return;
  }
  dataFile = SD.open("datalog.csv", FILE_WRITE);
}

void loop() {
  /* get time */
  RTC.read(tm);
  String tmstr = String(tm.Year + 1970) + "/" + String(tm.Month) + "/" + String(tm.Day) + " " + String(tm.Hour) + ":" + String(tm.Minute) + ":" + String(tm.Second);
  
  /* write to sd */
  if (dataFile) {
    dataFile.println(tmstr);
  }
  
  delay(1000);
}
