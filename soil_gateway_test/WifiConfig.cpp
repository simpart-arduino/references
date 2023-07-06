#include <WiFi.h>
#include <string.h>
#include "WifiConfig.h"
#include "common.h"
#include "eeprom.h"

using namespace std;

class SoilCharaCallback: public BLECharacteristicCallbacks {
  public:
    WifiConfig *wconf;

    SoilCharaCallback(){}
    SoilCharaCallback(WifiConfig *w) {
      wconf = w;
    }
    
    void onWrite(BLECharacteristic *ble_char) {
      COM_DEBUG("write uuid:" + ble_char->getUUID().toString());
      COM_LOG("value: " + ble_char->getValue());
      
      std::string uuid = ble_char->getUUID().toString();
      wconf->setCharacteristic(uuid, ble_char->getValue());
    }
};


WifiConfig::WifiConfig (void) : BLEPeripheral (WIF_DEVICE_NAME) {
  BLEPeripheral::setService(BLE_SERVICE_UUID);                   // set service uuid
  BLEPeripheral::setCharacteristic(BLE_WIFSSID_CHARUUID, "");    // ssid
  BLEPeripheral::setCharacteristic(BLE_WIFPASS_CHARUUID, "");    // password
  BLEPeripheral::setCharacteristic(BLE_WIFCONN_CHARUUID, "");    // connect event
  BLEPeripheral::setCharaCallback(new SoilCharaCallback(this));
}


/**
 * connect to wifi from eeprom config
 * 
 * @return bool true: successful connection
 *              false: failed connection
 */
bool WifiConfig::romConnect (void) {
  bool ret = false;
  T_WifiConfig cnf;
  
  memset(&cnf, 0x00, sizeof(T_WifiConfig));
    
  /* get wifi config */
  eep_wifi_read(&cnf);
  //eep_read((u_char *)EEP_ADDR_WIFICONF, (u_char *)&cnf, sizeof(T_WifiConfig));

  /* check config */
  //if ( (0 == cnf.w_count) ||                          // if not initialized, it may be a random value
  //     (0 == strnlen(cnf.ssid, WIF_SSID_SIZE-1))) {
  //  COM_LOG("config has not been set yet");
  //  return false;
  //}
  COM_DEBUG(cnf.ssid);
  COM_DEBUG(cnf.pass);
  
  /* connect to wifi */
  int limit = 0;
  WiFi.begin();
  while (WL_CONNECTED != WiFi.status()) {
    delay(100);
    limit++;
    if (50 < limit) {
      break;
    }
  }
  if(WL_CONNECTED != WiFi.status()) {
    COM_LOG("could not connect to wifi");
    return false;  
  }
    
  COM_LOG("connect to wifi");
  return true; 
}

/**
 * start BLE service for wifi connection.
 */
bool WifiConfig::bleSetup (void) {
  BLEPeripheral::start();

  // wait for connect event
  std::string conn = BLEPeripheral::getValue(BLE_WIFCONN_CHARUUID);
  while (0 != conn.compare("connect")) {
    delay(1000);
    conn = BLEPeripheral::getValue(BLE_WIFCONN_CHARUUID);
  }
  COM_LOG("get connect event");

  // wait for disconnect ble
  while (!BLEPeripheral::isConnected()) {
    delay(1000);
  }

  const char *ssid = (const char *) BLEPeripheral::getValue(BLE_WIFSSID_CHARUUID).c_str();
  const char *pass = (const char *) BLEPeripheral::getValue(BLE_WIFPASS_CHARUUID).c_str();  
  WiFi.begin(ssid, pass);
  delay(5000);  // magic
  int limit = 50;
  while (WL_CONNECTED != WiFi.status()) {
    delay(100);
    limit++;
    if (50 < limit) {
      break;
    }
  }

  if (WL_CONNECTED != WiFi.status()) {
    return false;
  }

  COM_LOG("wifi connection is successful");
  
  // save wifi config
/* failed..
  T_WifiConfig wcnf;
  memset(&wcnf, 0x00, sizeof(T_WifiConfig));
  strncpy((char *) wcnf.ssid, ssid, WIF_SSID_SIZE);
  strncpy((char *) wcnf.pass, pass, WIF_PASS_SIZE);
  if (!eep_wifi_write(&wcnf)) {
    return false;
  }
*/
  return true;
}

bool WifiConfig::isFinished (void) {
  return is_finished;
}

void WifiConfig::setFinish (bool f) {
  is_finished = f;
}
