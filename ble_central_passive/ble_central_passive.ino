#include <BLEDevice.h>

/* UUID定義 */
static BLEUUID serviceUUID("4FAFC201-1FB5-459E-8FCC-C5C9C331914A");   // サービスのUUID
static BLEUUID charUUID("BEB5483E-36E1-4688-B7F5-EA07361B26A8");      // characteristic UUID for moisture

boolean found_sens = false;
BLEAdvertisedDevice *soil_sens = NULL;

// callback for received advertise packets 
class advertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice ad_dev) {
        // check uuid of advertised device
        if (ad_dev.haveServiceUUID() && ad_dev.getServiceUUID().equals(serviceUUID)) {
          BLEDevice::getScan()->stop();
          
          Serial.println("connect to soil sensor.");
          soil_sens  = new BLEAdvertisedDevice(ad_dev);
          found_sens = true;
          
        }
    }
};

void setup() {
  Serial.begin(19200);
  BLEDevice::init("");
  
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new advertisedDeviceCallbacks());
  
  // start scan by passive mode
  pBLEScan->setActiveScan(false);
  pBLEScan->start(0);
}

void loop() {
  if (true == found_sens) {    
    found_sens = false;
    
    BLEClient* pClient = BLEDevice::createClient();
    // Connect to the remove BLE Server.
    if (false == pClient->connect(soil_sens)) {
      BLEDevice::getScan()->start(0);
      return;
    }
    Serial.println(" - Connected to server");
    
    BLERemoteService* soilServ = pClient->getService(serviceUUID);
    BLERemoteCharacteristic* mosChara = soilServ->getCharacteristic(charUUID);
    if (NULL == mosChara) {
      Serial.print("could not found characteristic UUID");
      pClient->disconnect();
      return;
    }
    
    // Read the value of the characteristic.
    if (mosChara->canRead()) {
      std::string value = mosChara->readValue();
      Serial.print("moisture value: ");
      
      const char *m_chr = value.c_str();
      Serial.printf("%x%x", *m_chr, *(m_chr+1));
      Serial.println();
    } else {
      Serial.println("could not read characteristic");
    }
    
    pClient->disconnect();
    BLEDevice::getScan()->start(0);
    
  }

  delay(80);
    
}    
/* end */
