#include <string.h>
#include <WiFi.h>
#include "ESP32REST.h"
#include "base64.h"

#define D_WIFI_SSID  "satomari24"
#define D_WIFI_PASS  "Sa1To0Ma1Ri9"

#define POST_HOST "kznjh7qye5.execute-api.ap-northeast-1.amazonaws.com"
#define POST_PATH "/prod"

ESP32REST *rest;

void setup() {
  WiFi.begin((char *) D_WIFI_SSID, (char *) D_WIFI_PASS);
  while (WL_CONNECTED != WiFi.status()) {}

  rest = new ESP32REST(POST_HOST, POST_PATH);
  Serial.begin(115200);

  
  String base64_img = rest->send("");

  size_t out_len = 0;
  unsigned char* b_img = base64_decode(
                         (const char *) base64_img.c_str(),
                         strnlen(base64_img.c_str(), 10000),
                         out_len
                       );
  //Serial.println(out_len);

  free(b_img);
}

void loop() {
  
//  String ret = post->send("");
//  Serial.println(ret);
//  delay(5000);
}
