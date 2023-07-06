#include <WiFi.h>

int p_num = 18;
const char SSID[] = "satomari24";
const char PASSWORD[] = "Sa1To0Ma1Ri9";


void setup() {
  Serial.begin(19200);
  pinMode(p_num,OUTPUT);
  //esp_sleep_enable_timer_wakeup(10000000);
  
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println(WiFi.localIP());
}


float get_wind () {
  unsigned short wind = 0;
  float fwind = 0;
  int rdat = 0;
  for (int i=0;i < 100;i++) {
    if (Serial.available()) {
      rdat = Serial.read();
      if (0x24 == rdat) {
        rdat = Serial.read();
        if (0x62 == rdat) {
          rdat = Serial.read();
          wind = (rdat << 8);
          rdat = Serial.read();
          
          wind |= rdat;
          fwind = (float) (wind * 64) / (float) 65536;
//        Serial.printf("%f[m/s]",fwind);
//        Serial.println("");
          return fwind;
        }
      }
    }
    delay(10);
  }
  return -1;  
}



void loop() {
  digitalWrite(p_num,LOW);
  delay(11000);
  float wind = 0;
  wind = get_wind();
  

  digitalWrite(p_num,HIGH);
  
}
