#include "common.h"
#include "eeprom.h"

T_WifiConfig g_wifcnf_buff;

/**
 * read data from eeprom
 */
void eep_read (u_char *addr, u_char *out, size_t siz) {
  if (NULL == out) {
    throw "invalid parameter";
  }
  
  u_char dat = 0;
  for (int i=0; i < siz ;i++) {
    EEPROM.get<u_char>((int) addr+i, dat);
    memcpy(out+i, &dat, sizeof dat);
  }
  
}


bool eep_wifi_read (T_WifiConfig *out) {
  if (NULL == out) {
    return false;
  }
  
  EEPROM.get<T_WifiConfig>(EEP_ADDR_WIFICONF, g_wifcnf_buff);
  COM_LOG("read wifi config from eeprom");
  COM_DEBUG(g_wifcnf_buff.ssid);
  COM_DEBUG(g_wifcnf_buff.pass);
  
  memcpy(out, &g_wifcnf_buff, sizeof(T_WifiConfig));

  return true;
}

bool eep_wifi_write (T_WifiConfig *cnf) {
  bool ret;
  if (NULL == cnf) {
    return false;
  }
  memcpy(&g_wifcnf_buff, cnf, sizeof(T_WifiConfig));
  
  EEPROM.put<T_WifiConfig>(EEP_ADDR_WIFICONF, g_wifcnf_buff);
  if (EEPROM.commit()){
    COM_ERROR("failed write eeprom");
    return false;
  }
    
  COM_LOG("write config to eeprom");
  COM_DEBUG(g_wifcnf_buff.ssid);
  COM_DEBUG(g_wifcnf_buff.pass);

  
  return true;
}

/* end of file */
