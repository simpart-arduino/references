#include <WiFi.h>
#include "bluetooth.h"
#include "common.h"
#include "WifiConfig.h"


SoilCentral *central;

void setup() {
  try {
    com_init();
    
    /* check wifi connection */
//    WifiConfig *wcnf = new WifiConfig();
//    if (false == wcnf->romConnect()) {
//      COM_LOG("setup wifi connection");
//      if (false == wcnf->bleSetup()) {
//        throw new Exception("failed wifi setup");
//      }
//      COM_LOG("finished wifi connection");
//    }
//    
//    if (WL_CONNECTED == WiFi.status()) {
//      WiFi.disconnect();
//      
//    }
    

    
    central = new SoilCentral();
    central->start();
    
  } catch (Exception *e) {
    COM_ERROR("!!!!!!!!!catched exception restart system!!!!!!!!!");
    ESP.restart();
  }
}


int mos_val = 0;
void loop() {
  
  if (true == central->isFound()) {
    COM_LOG("sensor is found");
    mos_val = central->getMoisture();
    if (-1 != mos_val) {
      // chack data etc..
      
    }
  }
  // restart scan
  central->start();
  
  
}
