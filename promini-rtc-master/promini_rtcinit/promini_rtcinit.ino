#include <Wire.h>
#include <DS3232RTC.h>

void setup(void) {
    setTime(15, 26, 00, 15, 10, 2021);   // set the system time in Arduino [Hour, Min, Sec, Day, Month, Year]
    RTC.set(now());   // set the RTC time according to Arduino (Need to activate only when the RTC module has to be re-set)
    
    if(timeStatus() != timeSet) {
       /* Unable to sync with the RTC */
    }
}

void loop(void){

}
