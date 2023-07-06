/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
*/
#include "Arduino_JSON.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <WiFi.h>

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

class CharaCB: public BLECharacteristicCallbacks {
  void onRead(BLECharacteristic *pCharacteristic) {
      Serial.println("read!");
//    pCharacteristic->setValue("Hello World!");
  }

  void onWrite(BLECharacteristic *pCharacteristic) {
    Serial.println("write!");
    std::string value = pCharacteristic->getValue();
    //Serial.printf("length:%d¥n",value.length());
    //Serial.println(value.c_str());
    
    JSONVar wcnf = JSON.parse(value.c_str());
    if(wcnf.hasOwnProperty("access") && wcnf.hasOwnProperty("password")){
      //Serial.println((const char*)wcnf["access"]);
      //wifi_connect((const char*)wcnf["access"],(const char*)wcnf["password"]);
      WiFi.begin((const char*)wcnf["access"],(const char*)wcnf["password"]);
      Serial.println("wifi connect!!!");
    }
  }
};

void init_ble() {
  Serial.println("initialize bluetooth");
  
  BLEDevice::init("wind_1");
  BLEServer         *pServer         = BLEDevice::createServer();
  BLEService        *pService        = pServer->createService(SERVICE_UUID);
  
  
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
  pCharacteristic->setCallbacks(new CharaCB());
  pServer->setCallbacks(new BLECallbacks());
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  pairing();
}

void pairing() {
  ble_connected = false;
  BLEDevice::startAdvertising();
  
  Serial.printf("pairing");
  while(!ble_connected) {
    delay(1000);
    Serial.printf(".");
  }
  Serial.println("connected!");
}


void setup() {
  Serial.begin(19200);
  init_ble();
}



void loop() {
  if (ble_connected) {
//    snprintf(val,sizeof val,"%d",get_wind());
//    //Serial.printf("set value %s¥n",val);
//    pCharacteristic->setValue(val);
  } else {
    pairing();
  }

  delay(1000);
}
