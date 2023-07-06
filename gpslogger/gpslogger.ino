#include <libGYSFDMAXB.h>

#include "network.h"
#include <string>
#include <libGYSFDMAXB.h>

#define D_API_ADDGPS_HOST "c848x8f1oh.execute-api.ap-northeast-1.amazonaws.com"

GYSFDMAXB *gps;

void setup() {
  nw_init();
  Serial.begin(9600);
  gps = new GYSFDMAXB();
}

void loop() {
  
  PosInfo pos;
  
  if (gps->getpos(&pos)) {
    float lati  = pos.latitude["value"];   // latitude value (ddmm.mmmm)
    float longi = pos.longitude["value"];  // longitude value (ddmm.mmmm)

    //Serial.println(lati);
    //Serial.println(longi);
    
    std::string pos_jsn = "{\"latitude\":" + std::to_string(lati) + ",\"longitude\":" + std::to_string(longi) + "}";    
    String resp = send_post(D_API_ADDGPS_HOST, "/prod", pos_jsn.c_str());
    Serial.println(resp);
  }
  
  delay(1000);
}
