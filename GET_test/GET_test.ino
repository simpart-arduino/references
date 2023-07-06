#include <WiFi.h>
 
const char* ssid     = "satomari24";
const char* password = "Sa1To0Ma1Ri9";
 
// URL(例) http://www.example.com/iot/get.php?value=1&string=ABCDE
const char* host = "54.238.76.124";
const char* path = "/otqr";
 
void setup() {
  Serial.begin(115200);
  
  // WIFI_AP, WIFI_STA, WIFI_AP_STA or WIFI_OFF
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFiに接続しました。");  
  Serial.print( "IPアドレス： "); 
  Serial.println( WiFi.localIP());
  Serial.print( "ゲートウェイ： "); 
  Serial.println(WiFi.gatewayIP());
  Serial.print( "サブネットマスク： "); 
  Serial.println(WiFi.subnetMask());
 
  delay(1000);
 
  WiFiClient client;
  const int httpPort = 5000;
  if (!client.connect(host, httpPort)) {
    Serial.println("接続に失敗しました。");
    return;
  }
  
  // サーバにリクエストを送信(GET)
  client.print(String("GET ") + path + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
               
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> タイムアウトしました。");
      client.stop();
      return;
    }
  }
 
  // HTTPヘッダを含むデータの読み取り(1文字毎) 
  String raw = "";
  while (client.available()) {
    char c = client.read();
    raw = raw +c;
  }  
 
  // HTTPヘッダの削除
  int index = raw.indexOf("\r\n\r\n");
  raw = raw.substring(index + 4);
 
  if(raw == "OK"){
    Serial.println("GETの成功");
  }else{
    Serial.println("GETの失敗");
  }
  
  Serial.println("-----");
  // ファイルサイズ
  Serial.print(raw.length()); 
  Serial.println(" byte"); 
  Serial.println("-----");
  // ファイルの生データ
  Serial.println(raw);
  Serial.println("-----");
  
  // Wifiの切断
  WiFi.disconnect(); 
}
 
void loop() {
  // none
}
