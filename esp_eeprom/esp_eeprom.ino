#include <EEPROM.h>

// int: 32bit (4byte)
int a0 = 11, a1 = 22, a2 = 33, a3 = 44; // テストデータ
const int N = 4; // 4つの変数(a0,a1,2,a3)
int data[N];

void eeprom_write() {
  data[0] = a0;
  data[1] = a1;
  data[2] = a2;
  data[3] = a3;
  int n = 0;
  for (int i = 0; i < N; i++) {
    EEPROM.put(n, data[i]);
    n += 4; // 4バイト毎
  }
  EEPROM.commit(); // EEPROMに書き込み確定
}

void eeprom_read() {
  int n = 0;
  for (int i = 0; i < N; i++) {
    EEPROM.get(n, data[i]); // EEPROMより読み込み
    n += 4; // 4バイト毎
  }
  Serial.println(data[0]);
  Serial.println(data[1]);
  Serial.println(data[2]);
  Serial.println(data[3]);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  EEPROM.begin(16); // 16byte = 4変数 x 4バイト(int=32bit)
  eeprom_write(); // 実行後
  eeprom_read();
}

void loop() {
}
