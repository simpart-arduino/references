
#define END_PIN 12

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(END_PIN, OUTPUT);
  digitalWrite(END_PIN, LOW);
}

void loop() {
  delay(5000);
  digitalWrite(END_PIN, HIGH);
  Serial.println("end");
}
