#ifndef __ESP32REST_H__
#define __ESP32REST_H__

#include <WiFiClientSecure.h>


class ESP32REST {
  private:
    String host;
    String path;
    WiFiClientSecure client;
    
  public:
    String send (String);
    ESP32REST(const char *, const char *);

};

#endif
