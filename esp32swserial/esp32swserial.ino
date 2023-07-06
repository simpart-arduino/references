
HardwareSerial rnSerial(2);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  //Serial1.begin(19200, SERIAL_8N1, 33,34);
  rnSerial.begin(19200, SERIAL_8N1, 16,17);
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

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  Serial.flush();
  rnSerial.write("test");
  Serial.flush();
  delay(100);
  serial_read();


}
