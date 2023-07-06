#include <string.h>
#include "common.h"
#include "bluetooth.h"

using namespace std;


class BleServCB: public BLEServerCallbacks {
  private:
    BLEPeripheral *peripheral;
    
  public:
    BleServCB (BLEPeripheral *pp) {
      peripheral = pp;
    }
  
    void onConnect(BLEServer* pServer) {
      peripheral->connEvent(true);
    };
    
    void onDisconnect(BLEServer* pServer) {
      peripheral->connEvent(false);
    }
};


BLEPeripheral::BLEPeripheral () {}
BLEPeripheral::BLEPeripheral (string n) {
  d_name = n;
}

void BLEPeripheral::setService (string s) {
  srv_uuid = s;
}
    
void BLEPeripheral::setCharacteristic (string c, string v) {
  char_uuid[c] = v;
}

std::string BLEPeripheral::getValue (string c) {
  return char_uuid[c];
}

void BLEPeripheral::setCharaCallback (BLECharacteristicCallbacks *cb) {
  char_cb = cb;
}

void BLEPeripheral::start (void) {
  COM_DEBUG("start ble peripheral");
  
  BLEDevice::init(d_name);
  BLEServer  *ble_srv = BLEDevice::createServer();
  BLEService *ble_svc = ble_srv->createService(srv_uuid);

  /* set callback every caracteristics */
  BLECharacteristic *ble_chara = NULL;
  for(auto itr = char_uuid.begin(); itr != char_uuid.end(); ++itr) {
    COM_DEBUG("set characteristic: " + itr->first);
    ble_chara = ble_svc->createCharacteristic(
                       itr->first.c_str(),
                       BLECharacteristic::PROPERTY_READ   |
                       BLECharacteristic::PROPERTY_WRITE  |
                       BLECharacteristic::PROPERTY_NOTIFY |
                       BLECharacteristic::PROPERTY_INDICATE
                     );
    ble_chara->setCallbacks(char_cb);
  }
  /* start service */
  ble_srv->setCallbacks(new BleServCB(this));
  ble_svc->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  COM_LOG("peripheral service uuid:" + this->srv_uuid);
  pAdvertising->addServiceUUID(this->srv_uuid);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  
  COM_LOG("start advertising");
}


void BLEPeripheral::connEvent (bool con_flg) {
  COM_LOG((con_flg) ? "connect peripheral" : "disconnect peripheral");
  is_connected = con_flg;
}

bool BLEPeripheral::isConnected (void) {
  return is_connected;
}
