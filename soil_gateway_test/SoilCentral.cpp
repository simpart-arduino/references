#include "common.h"
#include "bluetooth.h"
#include <HardwareSerial.h>
#include <BLEDevice.h>

static BLEUUID SOIL_SERV_UUID("4FAFC201-1FB5-459E-8FCC-C5C9C331914A");
static BLEUUID SOIL_CHAR_UUID("BEB5483E-36E1-4688-B7F5-EA07361B26A8"); // characteristic UUID for moisture

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
        if (ad_dev.haveServiceUUID() && ad_dev.getServiceUUID().equals(SOIL_SERV_UUID)) {
          BLEDevice::getScan()->stop();
          COM_DEBUG("found out soil sensor device.");
          
          central->isFound(true);
          central->setAdvertise(new BLEAdvertisedDevice(ad_dev));
          
        }
    }
};


void SoilCentral::start (void) {
  isFound(false);
  if (false == init_flg) {
    BLEDevice::init("");
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new DevCallback(this));

    // start scan by passive mode
    pBLEScan->setActiveScan(false);
    init_flg = true;

    pBLEScan->start(0);
    
  } else {
    BLEDevice::getScan()->start(0);
    COM_DEBUG("restart scan");
  }
}


int SoilCentral::getMoisture (void) {
  int ret_val = -1;
  //isFound(false);
  
  BLEClient* pClient = BLEDevice::createClient();
  /* connect to the soilsensor */
  if (false == pClient->connect(soil_sens)) {
    return -1;
  }
  COM_DEBUG("connected to soilsensor");
  
  BLERemoteService* soilServ = pClient->getService(SOIL_SERV_UUID);
  BLERemoteCharacteristic* mosChara = soilServ->getCharacteristic(SOIL_CHAR_UUID);
  if (NULL == mosChara) {
    COM_ERROR("could not found characteristic UUID");
    pClient->disconnect();
    return ret_val;
  }
  
  COM_DEBUG("uuid check is successfully");
  
  // Read the value of the characteristic.
  if (mosChara->canRead()) {
    std::string value = mosChara->readValue();
    Serial.print("moisture value: ");
    
    const char *m_chr = value.c_str();
    ret_val = (*m_chr << 8) + *(m_chr+1);
    Serial.println(ret_val);
  } else {
    Serial.println("could not read characteristic");
    pClient->disconnect();
    return ret_val;
  }
  pClient->disconnect();
  return ret_val;
}


bool SoilCentral::isFound (void) {
  return is_found;
}

void SoilCentral::isFound (bool flg) {
  is_found = flg;
}

void SoilCentral::setAdvertise (BLEAdvertisedDevice *adv_dev) {
  soil_sens = adv_dev;
}
