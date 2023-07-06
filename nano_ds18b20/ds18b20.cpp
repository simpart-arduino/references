#include "ds18b20.h"
#include <DallasTemperature.h>
#include <OneWire.h>

/* define sensors */
OneWire oneWire_1(14);
DallasTemperature sensor_1(&oneWire_1);
OneWire oneWire_2(2);
DallasTemperature sensor_2(&oneWire_2);
OneWire oneWire_3(3);
DallasTemperature sensor_3(&oneWire_3);
OneWire oneWire_4(4);
DallasTemperature sensor_4(&oneWire_4);
OneWire oneWire_5(5);
DallasTemperature sensor_5(&oneWire_5);
OneWire oneWire_6(6);
DallasTemperature sensor_6(&oneWire_6);
OneWire oneWire_7(7);
DallasTemperature sensor_7(&oneWire_7);
OneWire oneWire_8(8);
DallasTemperature sensor_8(&oneWire_8);
OneWire oneWire_9(9);
DallasTemperature sensor_9(&oneWire_9);

void ds18b20_init (void) {
  sensor_1.setResolution(SENSER_BIT);
  sensor_2.setResolution(SENSER_BIT);
  sensor_3.setResolution(SENSER_BIT);
  sensor_4.setResolution(SENSER_BIT);
  sensor_5.setResolution(SENSER_BIT);
  sensor_6.setResolution(SENSER_BIT);
  sensor_7.setResolution(SENSER_BIT);
  sensor_8.setResolution(SENSER_BIT);
  sensor_9.setResolution(SENSER_BIT);
}

int ds18b20_getlist(float *out) {
  if (NULL == out) {
    return -1;
  }
  
  sensor_1.requestTemperatures();
  sensor_2.requestTemperatures();
  sensor_3.requestTemperatures();
  sensor_4.requestTemperatures();
  sensor_5.requestTemperatures();
  sensor_6.requestTemperatures();
  sensor_7.requestTemperatures();
  sensor_8.requestTemperatures();
  sensor_9.requestTemperatures();

  //Serial.println(sensor_8.getTempCByIndex(0));
  out[0] = sensor_1.getTempCByIndex(0);
  out[1] = sensor_2.getTempCByIndex(0);
  out[2] = sensor_3.getTempCByIndex(0);
  out[3] = sensor_4.getTempCByIndex(0);
  out[4] = sensor_5.getTempCByIndex(0);
  out[5] = sensor_6.getTempCByIndex(0);
  out[6] = sensor_7.getTempCByIndex(0);
  out[7] = sensor_8.getTempCByIndex(0);
  out[8] = sensor_9.getTempCByIndex(0);
  
  return 0;
}
/* end of file */
