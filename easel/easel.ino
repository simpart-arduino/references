void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  Serial1.begin(19200);
  delay(5000);
  rcv_easel();
  init_easel();
  delay(5000);
}

int send_cmd (char *cmd) {
  // send
  while (*cmd != '\0') {
    Serial.print(*cmd);
    Serial1.write(*cmd++);
  }
  Serial.println("");
  
  delay(10);

  rcv_easel();
    
  return 0;
}

void rcv_easel () {
  // receive
  int rcnt = Serial1.available();
  if (0 < rcnt) {
    for (int i=0;i < rcnt;i++) {
      Serial.print((char) Serial1.read());
    }
  } else {
    Serial.println("no rcv");  
  }

}

int init_easel () {  
  send_cmd((char*) "2\r\n");     //processorモードに移行  
  send_cmd((char*) "z\r\n");     //送信モード開始 
  return 0;
}

void loop() {
  //rcv_easel();

  send_cmd((char*) "test msg\r\n");
  
  delay(1000);
}
