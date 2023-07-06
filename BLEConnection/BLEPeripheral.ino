/*
 * File:      BLEPeripheral.ino
 * Function:  BLEコネクション通信のペリフェラルとして動作します。
 *            セントラルからの接続を待って温湿度の計測と送信を開始します。以降はタイマーに設定
 *            した間隔に従って計測と送信を繰り返し、接続が切れると接続待ちの状態に戻ります。
 *            プッシュボタンで模擬的に異常事態の発生を通知。押すと異常発生のシグナルを送信します。
 *            一方で、セントラルからのシグナルを受信します。ブリンクの指示を受けるとLEDを点灯また
 *            は消灯に反転し、停止の指示を受けるとLEDを点滅させて計測・送信を中止します。この状態
 *            はリセットによって解除されます。
 * Date:      2019/05/10
 * Author:    M.Ono
 * 
 * Hardware   MCU:  ESP32 (DOIT ESP32 DEVKIT V1 Board)
 *            ブレッドボードに上記開発ボードと温湿度センサー、プッシュボタン、LEDを配線
 *            温湿度センサーはDHT11 
 */
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <Ticker.h>                 // タイマー割り込み用
#include "DHT.h"                    // DHTセンサー用

/* 基本属性定義  */
#define SERVER_NAME "ESP32_BLE"     // サーバー名
#define SPI_SPEED   115200          // SPI通信速度
#define DHTTYPE     DHT11           // DHTセンサーの型式

/* シグナル種別 */
#define SIGNAL_ERROR    'E'         // (異常発生:Error)
#define SIGNAL_ACK      'A'         // (了解：Acknoledge)
#define SIGNAL_STOP     'S'         // (停止：Stop)
#define SIGNAL_BLINK    'B'         // (点滅：Blink)

/* UUID定義 */
#define SERVICE_UUID           "28b0883b-7ec3-4b46-8f64-8559ae036e4e"  // サービスのUUID
#define CHARACTERISTIC_UUID_TX "2049779d-88a9-403a-9c59-c7df79e1dd7c"  // 送信用のUUID
#define CHARACTERISTIC_UUID_RX "9348db8a-7c61-4c6e-b12d-643625993b84"  // 受信用のUUID

/* 通信制御用 */
BLECharacteristic *pCharacteristicTX;   // 送信用キャラクタリスティック
BLECharacteristic *pCharacteristicRX;   // 受信用キャラクタリスティック
bool deviceConnected = false;           // デバイスの接続状態
bool bInAlarm  = false;                 // デバイス異常判定

/* 通信データ */
struct tmpData {                        // 計測データ
    int   temperature;
    int   humidity;
};
struct tmpData  data;

struct tmpSignal {                      // シグナル
    char  hdr1;
    char  hdr2;
    char  signalCode;
};
struct tmpSignal signaldata = { 0xff, 0xff, 0x00 };

/* DHTセンサー*/
const int DHTPin = 14;                  // 接続ピン
DHT dht(DHTPin, DHTTYPE);               // DHTクラスの生成

/* LEDピン */
const int ledPin = 25;                  // 接続ピン
int ledState = LOW;                     // 状態

/* プッシュボタン */
const int buttonPin = 32;               // 接続ピン
int buttonState = LOW;                  // 状態
int lastButtonState = LOW;              // 直前の状態
unsigned long lastDebounceTime = 0;     // 直前の切替時刻
unsigned long debounceDelay = 50;       // デバウンスタイム(mSec.)

/* タイマー制御用 */
Ticker  ticker;
bool bReadyTicker = false;
const int iIntervalTime = 10;           // 計測間隔（10秒）

/*********************< Callback classes and functions >**********************/
// 接続・切断時コールバック
class funcServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        deviceConnected = true;
    }
    void onDisconnect(BLEServer* pServer) {
        deviceConnected = false;
    }
};

// シグナル受信時のコールバック
class funcReceiveCallback: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristicRX) {
        if (bInAlarm) {                       // 異常状態なら何もしない
            return;
        }
        // 受信メッセージならモニターに表示する
        std::string rxValue = pCharacteristicRX->getValue();
        if (rxValue.length() > 1) {
            Serial.print("Received data: ");
            for (int i = 0; i < rxValue.length(); i++) {
                Serial.print(rxValue[i]);
            }
            Serial.println();
        }
        // シグナルなら対応した動作をする
        else {
            switch (rxValue[0]) {
                case SIGNAL_ACK:
                    Serial.println("Receive data: [Ack!]");
                    break;
                case SIGNAL_STOP:   // 切断状態に移行し異常状態をセットする
                    Serial.println("Receive data: [Stop!]");
                    deviceConnected = false;
                    bInAlarm = true;
                    ledState = HIGH;
                    break;
                case SIGNAL_BLINK:  // LEDを点灯または消灯する
                    Serial.println("Receive data: [Blink!]");
                    if (ledState == LOW) {
                        ledState = HIGH;
                    } else {
                        ledState = LOW;
                    }
                    digitalWrite(ledPin, ledState);
                    break;
                default:
                    break;
            }
        }
    }
};

//  タイマー割り込み関数
void kickRoutine() {
    bReadyTicker = true;
}

/*****************************************************************************
 *                          Predetermined Sequence                           *
 *****************************************************************************/
void setup() {
    // 初期化処理を行ってBLEデバイスを初期化する
    doInitialize();
    BLEDevice::init(SERVER_NAME);

    // Serverオブジェクトを作成してコールバックを設定する
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new funcServerCallbacks());

    // Serviceオブジェクトを作成して準備処理を実行する
    BLEService *pService = pServer->createService(SERVICE_UUID);
    doPrepare(pService);

    // サービスを開始して、SERVICE_UUIDでアドバタイジングを開始する
    pService->start();
    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->start();
    // タイマー割り込みを開始する
    ticker.attach(iIntervalTime, kickRoutine);
    Serial.println("Waiting to connect ...");
}

void loop() {
    // 接続が確立されていて異常でなければ
    if (deviceConnected && !bInAlarm) {
        // タイマー割り込みによって主処理を実行する
        if (bReadyTicker) {
            doMainProcess();
            bReadyTicker = false;
        }
        /* プッシュボタンが押されたら異常状態を設定し、アラーム信号を送信する */
        if (isPushbuttonClicked()) {
            signaldata.signalCode = SIGNAL_ERROR;
            pCharacteristicTX->setValue((uint8_t*)&signaldata, sizeof(tmpSignal));
            pCharacteristicTX->notify();
            Serial.println("Send signal: [Error!]");
        }
    }
    // アラーム中はLEDを500mSecで点滅させる
    if (bInAlarm) {
        ledState = !ledState;
        digitalWrite(ledPin, ledState);
        delay(500);
    }
}

/*  初期化処理  */
void doInitialize() {
    Serial.begin(SPI_SPEED);
    pinMode(buttonPin, INPUT);        // GPIO設定：プッシュボタン        
    pinMode(ledPin, OUTPUT);          // GPIO設定：LED
    dht.begin();                      // センサーの起動
}

//  準備処理  //
void doPrepare(BLEService *pService) {
    // Notify用のキャラクタリスティックを作成する
    pCharacteristicTX = pService->createCharacteristic(
                      CHARACTERISTIC_UUID_TX,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
    pCharacteristicTX->addDescriptor(new BLE2902());

    // 受信用キャラクタリスティックを作成してシグナル受信時のコールバックを設定する
    pCharacteristicRX = pService->createCharacteristic(
                                           CHARACTERISTIC_UUID_RX,
                                           BLECharacteristic::PROPERTY_WRITE
                                         );
    pCharacteristicRX->setCallbacks(new funcReceiveCallback());
}

//  主処理ロジック  //
void doMainProcess() {
    // 温度と湿度を読み取る
    float t = dht.readTemperature();
    float h = dht.readHumidity();

    // 読み取り失敗なら再試行させる
    if (isnan(h) || isnan(t)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }
    // 構造体に値を設定して送信する
    data.temperature = (int)(t * 100);
    data.humidity = (int)(h * 100);
    pCharacteristicTX->setValue((uint8_t*)&data, sizeof(tmpData));
    pCharacteristicTX->notify();
    // シリアルモニターに表示する
    char bufValue[20];
    sprintf(bufValue, "%5.2fC, %5.2f%%", t, h);
    Serial.print("Send data: ");    Serial.println(bufValue);
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
