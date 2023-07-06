#include <SD.h>

const int chipSelect = 10;
File dataFile;

void setup() {
  if (!SD.begin(chipSelect)) {
    return;
  }

}

void loop() {
  /* write to sd */
  dataFile = SD.open("datalog.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.println("test");
  }
  
  dataFile.close();
  delay(1000);
}
