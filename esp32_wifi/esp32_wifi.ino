#include <WiFi.h>

void setup() {
  Serial.begin(19200);
  
  wl_status_t wifi_ret;
  wifi_ret = WiFi.begin("satomari24","Sa1To0Ma1Ri9");
  Serial.println(wifi_ret);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println(" connected");

}

void loop() {
  // put your main code here, to run repeatedly:

}
