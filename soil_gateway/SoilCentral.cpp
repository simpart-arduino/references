#include <BLEDevice.h>
#include "bluetooth.h"


class DevCallback: public BLEAdvertisedDeviceCallbacks {
  private:
    SoilCentral         *central;
    BLEAdvertisedDevice *device;
    bool is_started = false;
    
  public:
    DevCallback (SoilCentral *c) {
      central = c;
    }
    
    void onResult(BLEAdvertisedDevice ad_dev) {
        /* check uuid of advertised device */
        
        COM_DEBUG(ad_dev.getServiceUUID().toString().c_str());
        
//        BLEUUID *srv_uuid = new BLEUUID(BLE_SERVICE_UUID);
//        if (ad_dev.haveServiceUUID() && ad_dev.getServiceUUID().equals((BLEUUID) srv_uuid)) {
//          BLEDevice::getScan()->stop();
//          COM_DEBUG("found out soil sensor device.");
//
//          device = new BLEAdvertisedDevice(ad_dev);
//
//          
//        }
        
//        if (ad_dev.haveServiceUUID() && ad_dev.getServiceUUID().equals(serviceUUID)) {
//          BLEDevice::getScan()->stop();
//          
//          Serial.println("connect to soil sensor.");
//          soil_sens  = new BLEAdvertisedDevice(ad_dev);
//          found_sens = true;
//          
//        }
    }
};


bool SoilCentral::isConnected (void) {
  return false;
}

void SoilCentral::start (void) {

//  if (is_started) {
//    return;
//  }
  
  BLEDevice::init("");
  
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new DevCallback(this));
  
  // start scan by passive mode
  pBLEScan->setActiveScan(false);
  pBLEScan->start(0);
}

void SoilCentral::stop (void) {
  
}
