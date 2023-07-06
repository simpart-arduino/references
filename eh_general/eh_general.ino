#include <HardwareSerial.h>

int power_pin = 10;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  Serial.println("test");

  pinMode(power_pin,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(power_pin, LOW);
  delay(10000);
  digitalWrite(power_pin, HIGH);
  delay(1000);
}
