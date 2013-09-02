#ifndef DHT_H
#define DHT_H

/*
 * DHT22 Library for Netcruzer (MODTRONIX) by Andrew Frahn (EMMERTEX)
 * emmertex@gmail.com
*/

#include <HardwareProfile.h>

#define _timings 100 //Bits to be read, + extras (need 48 bits, 96 + 4 extra for fun)
#define _count 85  //Count Threshold, when above this, bit is a 1
#define _maxCount 130 //Count Max, when above this, end of data stream
#define _readInterval 800 //milliseconds (aditional 280ms in other timings)
            //Datasheet claims no more than 1 read per 2 seconds, tests have
            //proven 480ms reads worked, and updated find in a short period of
            //time.  Extended tests have not been done.
                //This is loop, not per sensor, so, if 2 sensors, about 4.6 seconds between reads for a 2 second interval.
#define _pin1Dir DIR_Y9  //Netcruzer Alias for Pin Direction (DIR_xx)
#define _pin1In PIN_Y9   //Netcruzer Alias for Pin Read (PIN_xx)
#define _pin1Out LAT_Y9  //Netcruzer Alias for Pin Write (LAT_xx)
#define _pin2Dir DIR_Y8  //Netcruzer Alias for Pin Direction (DIR_xx)
#define _pin2In PIN_Y8   //Netcruzer Alias for Pin Read (PIN_xx)
#define _pin2Out LAT_Y8  //Netcruzer Alias for Pin Write (LAT_xx)
#define _numSensors 2

extern BYTE sensor;

    static enum _DHTState
    {
        S_START,
        S_INIT,
        S_READ,
    } DHTState;

  void DHT_init();
  void DHT_run();
  WORD DHT_readT();
  WORD DHT_readH();
  BOOL DHT_health();
  

#endif
