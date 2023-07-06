#ifndef COMMON_H
#define COMMON_H

#include <string>

#define COM_SERIAL_SPEED 19200

#define COM_LV_NON -1
#define COM_LV_ERR 4
#define COM_LV_WRN 3
#define COM_LV_LOG 2
#define COM_LV_DBG 1

#define COM_ERROR(m) com_print(m,COM_LV_ERR);
#define COM_WARN(m)  com_print(m,COM_LV_WRN);
#define COM_DEBUG(m) com_print(m,COM_LV_DBG);
#define COM_LOG(m)   com_print(m,COM_LV_LOG);
//#define COM_DEBUG_INT(m) com_print(m,COM_LV_DBG);


typedef unsigned char u_char;
typedef unsigned int  u_int;

void com_init (void);
void com_print(std::string, int);

class Exception {
  private:
    std::string msg;

  public:
    Exception (void);
    Exception (std::string);
};

#endif
