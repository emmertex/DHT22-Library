#ifndef DHT_H
#define DHT_H

/*
 * DHT11/22 Library for Netcruzer (MODTRONIX) by Andrew Frahn (EMMERTEX)
 * emmertex@gmail.com
*/

#include <HardwareProfile.h>

#define _timings 100
#define _count 85
#define _pinDir DIR_Y9
#define _pinIn PIN_Y9
#define _pinOut OUT_Y9

    static enum _DHTState
    {
        S_START,
        S_INIT,
        S_READ,
    } DHTState;


  void DHTTask();
  WORD DHTTemperature();
  WORD DHTHumidity();
  BOOL DHTHealth();
  void initDHT();

#endif
