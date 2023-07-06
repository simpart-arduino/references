void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  delay(500);

  // start advertise  
  send_cmd((char *) "A");
  //delay(10000);
  
  // read moisture value
  int mval = 0;
  mval = analogRead(0);

  // set data
  char cmd[128] = { 0 };
  memset(cmd, 0x00, sizeof cmd);
  snprintf(cmd, sizeof cmd, "SUW,beb5483e36e14688b7f5ea07361b26a8,%d", mval);
  send_cmd(cmd);

  delay(50000);

  
  
  // stop advertise
  send_cmd((char *) "Z");
  
  // disconnect
  send_cmd((char *) "Y");
}

int send_cmd (char *cmd) {
  if (NULL == cmd) {
    return -1;
  }
  
  Serial.flush();
  Serial.write(cmd);
  Serial.println();
  delay(100);
  serial_read();

  return 0;
}


void serial_read () {
  // receive
  int rcnt = Serial.available();
  if (0 < rcnt) {
    for (int i=0;i < rcnt;i++) {
      Serial.read();
      //Serial.print((char) Serial.read());
    }
  } else {
    //Serial.println("no rcv");  
  }
}

void loop() {}
