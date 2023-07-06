
#include "BLEDevice.h"
#include "BLEServer.h"

#define DEV_NAME            "Raiten-GaCha"
#define SERVICE_UUID        "ec6ba320-e16b-11ea-87d0-0242ac130003"
#define CHARACTERISTIC_UUID "ec6ba321-e16b-11ea-87d0-0242ac130003"

void setup() {
  // put your setup code here, to run once:
  setupBLE();
}

void loop() {
  // put your main code here, to run repeatedly:

}


void setupBLE() {
  // This text will be sent by advertising.
//  String advManufacturerData = String(dataTemp, 1) + String(',') +
//                               String((int)dataHumid) + String(',') +
//                               String((int)dataIllum) + String(',') +
//                               String(dataBatt, 2);
//  ESP_LOGD(TAG, "Advertising Data: %s (Length: %d)", advManufacturerData.c_str(), advManufacturerData.length());
//
  /* Create the BLE Device */
  BLEDevice::init(DEV_NAME);

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
//  pServer->setCallbacks(new BLECharacteristicCallbacks());

  // Create a BLE Characteristic
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic;
  
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE |
                      BLECharacteristic::PROPERTY_WRITE
                    );
//  pCharacteristic->setCallbacks(new MyCharCallbacks());
//  pCharacteristic->addDescriptor(new BLE2902());
  pService->start();
  
  // Setup the advertising data
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
  oAdvertisementData.setFlags(0b0001 | 0b0100); // LE Limited Discoverable Mode | BR_EDR_NOT_SUPPORTED
  oAdvertisementData.setName(DEV_NAME);
  oAdvertisementData.setManufacturerData("test beacon"); // String to std::__cxx11:string
  pAdvertising->setAdvertisementData(oAdvertisementData);
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);

  pAdvertising->start();
//  ESP_LOGI(TAG, "Advertising started");
//
//  // continue advertising during this period.
//  delay(TIME_TO_AWAKE * 1000);
//
//  pAdvertising->stop();
//  ESP_LOGI(TAG, "Advertising stopped");
}
