#include "network.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>


#define D_WIFI_SSID "gps-nw"
#define D_WIFI_PASS "z7xjctzysumm3"


WiFiClientSecure client;

void nw_init (void) {
  WiFi.begin(D_WIFI_SSID, D_WIFI_PASS);
  while (WL_CONNECTED != WiFi.status()) {}
}

String send_post (String host, String url, String data) {

  if (WL_CONNECTED != WiFi.status()) {
    return "disconnect wifi";
  } else if (client.connected()) {
    return "already client connected";
  }
  
  client.setInsecure();
  if (client.connect(host.c_str(), 443)) {
    
    client.println("POST " + url + " HTTP/1.1");
    client.println("Host: " + host);
    client.println("User-Agent: ESP32");
    client.println("Connection: close");
    client.println("Content-Type: application/json;");
    client.print("Content-Length: ");
    client.println(data.length());
    client.println();
    client.println(data.c_str());

    delay(10);

    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        //Serial.println("headers received");
        break;
      }
    }
    return client.readString();
  } else {
    return "failed connection";
  }
}

/* end of file */
