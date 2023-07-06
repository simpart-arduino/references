
HardwareSerial rnSerial(2);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  //Serial1.begin(19200, SERIAL_8N1, 33,34);
  rnSerial.begin(19200, SERIAL_8N1, 16,17);
  delay(2000);
  send_cmd((char *) "SUW,beb5483e36e14688b7f5ea07361b26a8,100");
  send_cmd((char *) "A");
}

int send_cmd (char *cmd) {
  if (NULL == cmd) {
    return -1;
  }
  
  rnSerial.flush();
  rnSerial.write(cmd);
  rnSerial.println();
  delay(100);
  serial_read();

  return 0;
}


void serial_read () {
  // receive
  int rcnt = rnSerial.available();
  if (0 < rcnt) {
    for (int i=0;i < rcnt;i++) {
      Serial.print((char) rnSerial.read());
    }
  } else {
    Serial.println("no rcv");  
  }
}

int val = 0;
void loop() {
//  delay(1000);
//  
//  char cmd[256] = { 0 };
//  memset(cmd, 0x00, sizeof cmd);
//  val++;
//  
//  snprintf(cmd, (sizeof cmd) - 1, "SUW,beb5483e36e14688b7f5ea07361b26a8,%d", val);
//  send_cmd(cmd);
  
}
