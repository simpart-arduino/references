#include <HardwareSerial.h>
#include <string.h>
#include "common.h"
#include "eeprom.h"

u_char gcom_log_level = 0;
const char *gcom_log_index[] = { "", "DEBUG", "LOG", "WARN", "ERROR" };

/**
 * initialize process every functions
 */
void com_init (void) {
  Serial.begin(19200);
  Serial.println();
  Serial.println("======= start application =======");
  
  EEPROM.begin(EEP_AREA_SIZE);
  gcom_log_level = COM_LV_DBG;
}

/**
 * debug print function
 * output message to serial if COM_DEBUG is defined.
 * 
 * @param
 */
void com_print(std::string msg, int lv) {
  if (gcom_log_level <= lv) {
    Serial.print("[");
    Serial.print(gcom_log_index[lv]);
    Serial.print("] ");
    Serial.println(msg.c_str());
  }
}


Exception::Exception (void) {}
Exception::Exception (std::string m) {
  msg = m;
}
