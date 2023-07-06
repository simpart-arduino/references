/*
 * File:      BLECentral.ino
 * Function:  BLEコネクション通信のセントラルとして動作します。
 *            目的のペリフェラルが見つかると接続して、定期的に送信される計測データを受信し、シリ
 *            アルモニターおよびOLEDディスプレイに温度と湿度を表示します。計測データを受信する都
 *            度、ACK（Acknowledgement）シグナルを返送します。
 *            ペリフェラルからの異常シグナルが届くとLEDを点灯して、ペリフェラルに停止シグナルを送
 *            信します。プッシュボタンを押すと、異常発生時ならLEDを消灯して異常を解除します。異常
 *            状態でなければ、ペリフェラルのLEDの状態を反転させるブリンクシグナルを送信します。
 * Date:      2019/05/10
 * Author:    M.Ono
 * 
 * Hardware   MCU:  ESP32 (DOIT ESP32 DEVKIT V1 Board)
 *            ブレッドボードに上記開発ボードとOLEDディスプレイ、プッシュボタン、LEDを配線
 *            OLEDディスプレイはSSD1306  (0.96inch 128x64dot) 
 */
#include <BLEDevice.h>
#include <Wire.h>                   // For I2C interface
//#include <Adafruit_SSD1306.h>       // For SSD1306 display
//#include <Adafruit_GFX.h>           // For SSD1306 display

/* 基本属性定義  */
#define SPI_SPEED   115200          // SPI通信速度

/* シグナル種別 */
#define SIGNAL_ERROR    'E'         // (異常発生:Error)
#define SIGNAL_ACK      'A'         // (了解：Acknoledge)
#define SIGNAL_STOP     'S'         // (停止：Stop)
#define SIGNAL_BLINK    'B'         // (点滅：Blink)

///* OLEDディスプレイの設定 */
//#define SCREEN_WIDTH 128            // 幅　（単位：ピクセル）
//#define SCREEN_HEIGHT 64            // 高さ（単位：ピクセル）
//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

/* UUID定義 */
static BLEUUID serviceUUID("28b0883b-7ec3-4b46-8f64-8559ae036e4e");   // サービスのUUID
static BLEUUID CHARA_UUID_RX("2049779d-88a9-403a-9c59-c7df79e1dd7c"); // 受信用のUUID
static BLEUUID CHARA_UUID_TX("9348db8a-7c61-4c6e-b12d-643625993b84"); // 送信用のUUID

/* 通信制御用 */
BLERemoteCharacteristic* pRemoteCharacteristicRX;   // 受信用キャラクタリスティック
BLERemoteCharacteristic* pRemoteCharacteristicTX;   // 送信用キャラクタリスティック
BLEAdvertisedDevice* targetDevice;      // 目的のBLEデバイス
static boolean doConnect = false;
static boolean doScan = false;
bool deviceConnected = false;           // デバイスの接続状態
bool bInAlarm  = false;                 // デバイス異常判定
bool enableMeasurement = false;         // 計測情報が有効
bool bSkipAlarmCheck = false;           // 異常判定スキップ

/* 通信データ */
struct tmpData {                        // 計測データ
    int   temperature;
    int   humidity;
};
struct tmpData  data;

char* temperature;                      // 温度
char* humidity;                         // 湿度

/* LEDピン */
const int ledPin = 25;                  // 接続ピン
int ledState = LOW;                     // 状態

/* プッシュボタン */
const int buttonPin = 32;               // 接続ピン
int buttonState;                        // 状態
int lastButtonState = LOW;              // 直前の状態
unsigned long lastDebounceTime = 0;     // 直前の切替時刻
unsigned long debounceDelay = 50;       // デバウンスタイム(mSec.)

/*********************< Callback classes and functions >**********************/
// 接続・切断時コールバック
class funcClientCallbacks: public BLEClientCallbacks {
    void onConnect(BLEClient* pClient) {
    };
    void onDisconnect(BLEClient* pClient) {
        deviceConnected = false;
    }
};

// アドバタイジング受信時コールバック
class advertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        Serial.print("Advertised Device found: ");
        Serial.println(advertisedDevice.toString().c_str());

        // 目的のBLEデバイスならスキャンを停止して接続準備をする
        if (advertisedDevice.haveServiceUUID()) {
            BLEUUID service = advertisedDevice.getServiceUUID();
            Serial.print("Have ServiceUUI: "); Serial.println(service.toString().c_str());
            if (service.equals(serviceUUID)) {
                BLEDevice::getScan()->stop();
                targetDevice = new BLEAdvertisedDevice(advertisedDevice);
                doConnect = doScan = true;
            }
        }
    }
};

// Notify時のコールバック関数
static void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic,
                uint8_t* pData, size_t length, bool isNotify) {
    // アラーム信号ならLEDを点灯してアラーム状態を設定する
    if (*pData == *(pData+1) && *pData == 0xff && *(pData+2) == SIGNAL_ERROR) {
        digitalWrite(ledPin, HIGH);
        Serial.println("Received signal [Error!]");
        bInAlarm = true;
        return;
    }

    // 受信メッセージから温度と湿度を切り出して表示用に編集する
    memcpy(&data, pData, length);
    float t = data.temperature / 100.00;
    float h = data.humidity / 100.00;
    static char temp[10];
    static char humd[10];
    sprintf(temp, "%5.2fC", t);
    sprintf(humd, "%5.2f%%", h);
    temperature = (char*)temp;
    humidity = (char*)humd;
    enableMeasurement = true;
    Serial.print("Received data: ");  Serial.print(temperature);
    Serial.print(", ");  Serial.println(humidity);
}

/*****************************************************************************
 *                          Predetermined Sequence                           *
 *****************************************************************************/
void setup() {
    // 初期化処理を行ってBLEデバイスを初期化する
    doInitialize();
    BLEDevice::init("");
    Serial.println("Client application start...");

    // Scanオブジェクトを取得してコールバックを設定する
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new advertisedDeviceCallbacks());
    // アクティブスキャンで10秒間スキャンする
    pBLEScan->setActiveScan(true);
    pBLEScan->start(10);
}

void loop() {
    // アドバタイジング受信時に一回だけ準備処理を実行（ペリフェラルに接続）する
    if (doConnect == true) {
        if (doPrepare()) {
            Serial.println("Connected to the BLE Server.");
        } else {
            Serial.println("Failed to connect BLE server.");
        }
        doConnect = false;
    }
    // 接続状態において
    if (deviceConnected) {
        // 異常状態ならOLED表示を消してSIGNAL_STOPを送信する
        if (bInAlarm) {
            if (!bSkipAlarmCheck) {
//                displayValues("", "", "");
                pRemoteCharacteristicTX->writeValue(SIGNAL_STOP);
                Serial.println("Send data: [Stop!]");
                bSkipAlarmCheck = true;
            }
        }
        // そうでなくて測定値が有効ならOLEDに表示してSIGNAL_ACKを送信する
        else if (enableMeasurement)
        {
//            displayValues("DHT SENSOR", temperature, humidity);
            pRemoteCharacteristicTX->writeValue(SIGNAL_ACK);
            Serial.println("Send data: [ACK!]");
        }
        enableMeasurement = false;
        // プッシュボタンが押されていれば
        if (isPushbuttonClicked()) {
            // 異常状態でなければSIGNAL_BLINKを送信し、異常状態ならそれを解除する
            if (!bInAlarm) {
                pRemoteCharacteristicTX->writeValue(SIGNAL_BLINK);
                Serial.println("Send data: [Blink!]");
            } else {
                digitalWrite(ledPin, LOW);
                bInAlarm = false;
                bSkipAlarmCheck = false;
                Serial.println("Alarm state cleared.");
            }
        }
    } else if(doScan) {
        BLEDevice::getScan()->start(0);
    }
}

/*  初期化処理  */
void doInitialize() {
    Serial.begin(SPI_SPEED);
    pinMode(buttonPin, INPUT);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

//    // OLEDディスプレイを初期化する
//    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
//        Serial.println(F("** SSD1306 allocation failed **"));
//        for(;;); // 失敗すると処理を進ませない！
//    }
//    displayValues("DHT SENSOR", "", "");
    Serial.println("BLE Client start ...");
}

//  準備処理  //
bool doPrepare() {
    // クライアントを作成してコールバック関数を設定する
    BLEClient* pClient = BLEDevice::createClient();
    pClient->setClientCallbacks(new funcClientCallbacks());
    Serial.println(" - Created client.");

    // ペリフェラルと接続して
    pClient->connect(targetDevice);
    Serial.println(" - Connected to peripheral.");

    // サービスへの参照を取得する
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
        Serial.print("Failed to find serviceUUID: ");
        Serial.println(serviceUUID.toString().c_str());
        pClient->disconnect();
        return false;
    }
    Serial.println(" - Found target service.");

    // キャラクタリスティックへの参照を取得して
    pRemoteCharacteristicRX = pRemoteService->getCharacteristic(CHARA_UUID_RX);
    if (pRemoteCharacteristicRX == nullptr) {
      Serial.print("Failed to find characteristicUUID: ");
      Serial.println(CHARA_UUID_RX.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found characteristic CHARA_UUID_RX.");

    // Notifyのコールバック関数を割り当てる
    if(pRemoteCharacteristicRX->canNotify()) {
        pRemoteCharacteristicRX->registerForNotify(notifyCallback);
        Serial.println(" - Registered notify callback function.");
    }

    // Send用キャラクタリスティックの参照を取得する
    pRemoteCharacteristicTX = pRemoteService->getCharacteristic(CHARA_UUID_TX);
    if (pRemoteCharacteristicTX == nullptr) {
      Serial.print("Failed to find CHARA_UUID_TX: ");
      Serial.println(CHARA_UUID_TX.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found characteristic CHARA_UUID_TX.");

    digitalWrite(ledPin, LOW);
    deviceConnected = true;
    bInAlarm = false;
    return true;
}

/*****************************< Other functions >*****************************/
/* プッシュボタンが押されているかを通知する */
bool isPushbuttonClicked() {
    // 状態が変わっていたらタイマーをリセットして
    int state = digitalRead(buttonPin);
    if (state != lastButtonState) {
        lastDebounceTime = millis();
    }
    // デバウンス時間後にボタンの状態が変化したら応答する
    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (state != buttonState) {
            buttonState = state;
            if (buttonState == HIGH) {
                return true;
            }
        }
    }
    lastButtonState = state;      // ボタンの状態を退避する
    return false;
}

///* 測定値をOLEDディスプレイに表示する */
//void displayValues(char* line1, char* line2, char* line3) {
//    display.clearDisplay();
//    display.setTextSize(2);
//    display.setTextColor(WHITE);
//    display.setCursor(0,0);    display.print(line1);
//    display.setCursor(24,20);  display.print(line2);
//    display.setCursor(24,40);  display.print(line3);
//    display.display();
//}
