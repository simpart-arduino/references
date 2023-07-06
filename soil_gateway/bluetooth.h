#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <string>
#include <map>
#include "common.h"

using namespace std;

#define BLE_SERVICE_UUID     "64ac6782-77ab-bb50-2549-321900cc35b7"
#define BLE_WIFSSID_CHARUUID "8ae02df6-13a1-a643-afc6-e38015acdc17"  //! SSID
#define BLE_WIFPASS_CHARUUID "59eb653a-f480-031b-44c8-757de2a66a96"  //! PASSWORD
#define BLE_WIFCONN_CHARUUID "56c050ba-d16d-8330-12c8-a723b1117c6f"  //! Connect

class BLEPeripheral
{
  private:
    string d_name;                       /* device name */
    string srv_uuid;                     /* service uuid */
    std::map<string, string> char_uuid;  /* characteristic uuid */
    bool is_connected = false;
    //BLEServer  *ble_srv;
    BLECharacteristicCallbacks *char_cb;
    
  public:
    BLEPeripheral ();
    BLEPeripheral (string);
  
    void   setService (string);
    void   setCharacteristic (string, string);
    void   setCharaCallback (BLECharacteristicCallbacks *);
    string getValue (string);
    void   start (void);
    void   stop (void);
    void   connEvent (bool);
    bool   isConnected (void);
};


class SoilCentral
{
  public:
    bool isConnected (void);
    void start (void);
    void stop (void);
};

#endif
