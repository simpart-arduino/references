#include <WiFi.h>
//#include <WiFiClient.h>
#include <WiFiClientSecure.h>

#define D_WIFI_SSID "satomari24"
#define D_WIFI_PASS "Sa1To0Ma1Ri9"
#define GET_QR_HOST "s1y67ffjpa.execute-api.ap-northeast-1.amazonaws.com"
#define GET_QR_URL  "/prod"

// https://s1y67ffjpa.execute-api.ap-northeast-1.amazonaws.com/prod
// http://54.238.76.124:5000/otqr

//WiFiClient client;
WiFiClientSecure client;

void setup() {
  WiFi.begin(D_WIFI_SSID, D_WIFI_PASS);
  while (WL_CONNECTED != WiFi.status()) {}

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("start onetime QR");
  delay(1000);


  if (client.connected()) {
    Serial.println("already client connected");
    return;
  }
  
  client.setInsecure();
  if (client.connect(GET_QR_HOST,443)) {
    
    client.println("POST " + String(GET_QR_URL) + " HTTP/1.1");
    client.println("Host: " + String(GET_QR_HOST));
    client.println("User-Agent: ESP32");
    client.println("Connection: close");
    client.println("Content-Type: application/json;");
    client.println("Content-Length: 0");
    client.println();
    client.println();

    
//    client.println("GET " + String(GET_QR_URL) + " HTTP/1.1");
//    client.println("Path: " + String(GET_QR_URL));
//    client.println("Host: " + String(GET_QR_HOST));
//    client.println("User-Agent: ESP32");
//    //client.println("Connection: close");

//    client.println("GET /otqr HTTP/1.1");
//    client.println("Host: 54.238.76.124");
//    client.println("Connection: keep-alive");
//    client.println("Pragma: no-cache");
//    client.println("Cache-Control: no-cache");
//    client.println("Upgrade-Insecure-Requests: 1");
//    client.println("User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/89.0.4389.114 Safari/537.36");
//    client.println("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9");
//    client.println("Accept-Encoding: gzip, deflate");
//    client.println("Accept-Language: ja-JP,ja;q=0.9,en-US;q=0.8,en;q=0.7");
    
    delay(10);


    while (client.connected()) {
      
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        //Serial.println("headers received");
        break;
      }

      Serial.println(line);
    }
    
    Serial.println(client.readString());
  } else {
    Serial.println("failed connection");
  }

}
