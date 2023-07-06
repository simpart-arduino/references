/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914a"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLECharacteristic *pCharacteristic = NULL;
bool ble_connected = false;

class BLECallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      ble_connected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      ble_connected = false;
    }
};

void init_ble() {
  Serial.println("initialize bluetooth");
  
  
}


void setup() {
  Serial.begin(19200);
  /* initialize ble */
  BLEDevice::init("eh_test");
  BLEServer   *pServer         = BLEDevice::createServer();
  BLEService        *pService        = pServer->createService(SERVICE_UUID);
  
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
                    );
  pServer->setCallbacks(new BLECallbacks());
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  while(!ble_connected) {
    delay(1000);
    Serial.printf(".");
  }
}



//int sensor_val = 100;
char val[32]   = {0};

void loop() {
  if (ble_connected) {
    Serial.println("ble connected");
//    //snprintf(val,sizeof val,"%d",get_wind());
//    //Serial.printf("set value %s¥n",val);
    pCharacteristic->setValue("100");
    delay(1000);
  } else {
    Serial.println("ble does not connect");
//    pairing();
    //BLEDevice::startAdvertising();
  }

  esp_sleep_enable_timer_wakeup(3600000000);
  // ディープスリープ
  esp_deep_sleep_start();
  
}
