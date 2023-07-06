#include <DS3232RTC.h>

void setup() {
  Serial.begin(115200);
}

void loop() {
  tmElements_t tm;
  RTC.read(tm);
  Serial.print(tm.Year + 1970, DEC); //年は1970年からの年数で記録されるので1970を足して表示
  Serial.print("年");
  Serial.print(tm.Month, DEC);//月を表示
  Serial.print("月");
  Serial.print(tm.Day, DEC);//日を表示
  Serial.print("日");
  Serial.print(tm.Hour, DEC);//時を表示
  Serial.print("時");
  Serial.print(tm.Minute, DEC);//分を表示
  Serial.print("分");
  Serial.println(tm.Second, DEC);//秒を表示して改行

  delay(1000);//１間隔で表示させるため１秒待機
}
