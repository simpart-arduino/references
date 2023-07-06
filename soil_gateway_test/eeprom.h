#ifndef EEPROM_H
#define EEPROM_H

#include <EEPROM.h>
#include <cstddef>
#include "common.h"
#include "WifiConfig.h"

#define EEP_AREA_SIZE     128
#define EEP_HEAD_WIFICONF 0x0000
#define EEP_ADDR_WIFICONF 0x000f


/**
 * read value from eeprom
 * 
 * @param 
 * @param 
 */
void eep_read (u_char *, u_char *, size_t);

bool eep_wifi_read (T_WifiConfig *);
bool eep_wifi_write (T_WifiConfig *);
#endif
