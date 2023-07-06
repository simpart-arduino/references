#include "ESP32REST.h"

ESP32REST::ESP32REST (const char *h, const char *p) {
  if ((NULL == h) || (NULL == p)) {
    return;
  }
  host = String(h);
  path = String(p);
}

String ESP32REST::send (String prm) {
  try {
    if (client.connected()) {
      /* already client connected */
      return String("send error: already client connected");
    }
    
    client.setInsecure();
    Serial.println(host.c_str());
    
    if (client.connect(host.c_str(),443)) {
      
      client.println("POST " + path + " HTTP/1.1");
      client.println("Host: " + host);
      client.println("User-Agent: ESP32");
      client.println("Accept: image/*");
      client.println("Connection: close");
      client.println("Content-Type: application/json;");
      client.println("Content-Length: 0");
      client.println();
      //client.println(prm);
      
      delay(10);
          
      while (client.connected()) {
        
        String line = client.readStringUntil('\n');
        if (line == "\r") {
          //Serial.println("headers received");
          break;
        }
        
        //Serial.println(line);
      }
      
      return client.readString();
    } else {
      return String("failed connection");
    }
  } catch (std::exception e) {
    return String("send error: unknown");
  }
}
