#ifndef WIFICONFIG_H
#define WIFICONFIG_H

#include "bluetooth.h"


#define WIF_SSID_SIZE 256
#define WIF_PASS_SIZE 256
#define WIF_DEVICE_NAME "Soil-Gateway"


typedef struct Wif_Config {
  int w_count = 0;
  const char ssid[WIF_SSID_SIZE] = { 0 };
  const char pass[WIF_PASS_SIZE] = { 0 };
} T_WifiConfig;

class WifiConfig: public BLEPeripheral {
  public:
    bool romConnect (void);
    bool bleSetup (void);
    bool isFinished (void);
    void setFinish (bool);
    WifiConfig(void);
    
  private:
    bool is_finished = false;
        
};

#endif
