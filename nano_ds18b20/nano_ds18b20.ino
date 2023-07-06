#include "ds18b20.h"
#include <SPI.h>
#include <SD.h>
#include <DS3232RTC.h>

#define SENSOR_CNT 9
#define SD_CHIPSELECT 10
File dataFile;

void save_data (String);

void setup() {
  Serial.begin(9600);
  ds18b20_init();
  
  if (!SD.begin(SD_CHIPSELECT)) {
    Serial.println("Card failed, or not present");
    return;
  }

}

float temp_list[SENSOR_CNT] = {0};

void loop() {
  tmElements_t tm;
  RTC.read(tm);
  if (59 == tm.Second) {
    delay(100);
    return;
  }  
  
  String tmstr = String(tm.Year + 1970) + "/" + String(tm.Month) + "/" + String(tm.Day) + " " + String(tm.Hour) + ":" + String(tm.Minute) + ":" + String(tm.Second);

  /* get temprature */
  ds18b20_getlist(temp_list);  

  /* save data */
  String datstr = "";
  for (int i=0;i < SENSOR_CNT;i++) {
    datstr += "," + String(temp_list[i]);
  }

//  Serial.println(tmstr);
//  Serial.println(datstr);
  
  dataFile = SD.open("datalog.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.print(tmstr);
    dataFile.println(datstr);
  }  
  dataFile.close();
  
  /* loop the next minture */
  RTC.read(tm);
  //tmstr = String(tm.Year + 1970) + "/" + String(tm.Month) + "/" + String(tm.Day) + " " + String(tm.Hour) + ":" + String(tm.Minute) + ":" + String(tm.Second);
  
  unsigned int sec_diff = 60-tm.Second;
  delay((sec_diff*1000) - 300);
}



void save_data (String msg) {

  
  /* SDカードに書き込み */
  File dataFile = SD.open("datalog.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.println("test2");
  }
  dataFile.close();
}
