#include "libGYSFDMAXB.h"

GYSFDMAXB *gps;

void setup() {
  Serial.begin(9600);
  gps = new GYSFDMAXB();
}


void loop() {
  PosInfo pos;
  
  if (gps->getpos(&pos)) {
    float lati  = pos.latitude["value"];
    float longi = pos.longitude["value"];
    
    //Serial.println(lati);
    //Serial.println(longi);
    Serial.println(pos.dmmstr().c_str());
  }
  
  delay(1000);
}
