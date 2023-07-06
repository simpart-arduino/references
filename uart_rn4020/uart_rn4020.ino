//#include <string.h>

char ble_resp[64] = {0};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
}



void loop() {

  memset(&ble_resp[0], 0x00, sizeof ble_resp);
  
  int rcnt = Serial.available();
  if (0 < rcnt) {
    Serial.print("responce count:");
    Serial.println(rcnt);
    
    for (int i=0;i < rcnt;i++) {
      ble_resp[i] = Serial.read();
    }
    //Serial.print(ble_resp);
    
    if ((NULL != strstr(ble_resp, "CMD")) && (4 < rcnt)) {
      Serial.println("received CMD");
      /* received CMD */
      //Serial.write("r,1¥r¥n");
      char test[] = { 0x72, 0x2C, 0x31, 0x0A, 0x0D };
      Serial.write(test);
    }
    
  } else {
    delay(100);  
  }
}
