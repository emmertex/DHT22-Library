#ifndef DHT_H
#define DHT_H

/*
 * DHT22 Library for Netcruzer (MODTRONIX) by Andrew Frahn (EMMERTEX)

Copyright 2013 - Andrew Frahn (Emmertex)
 emmertex@gmail.com
 
 This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Australia License.
 To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/au/.
 
*/

#include <HardwareProfile.h>

#define _timings 120 //Bits to be read, + extras (need 48 bits, 96 + 4 extra for fun)
#define _count 85  //Count Threshold, when above this, bit is a 1
#define _maxCount 200 //Count Max, when above this, end of data stream
#define _readInterval 550 //milliseconds (aditional 280ms in other timings)
            //Datasheet claims no more than 1 read per 2 seconds, tests have
            //proven 480ms reads worked, and updated find in a short period of
            //time.  Extended tests have not been done.
                //This is loop, not per sensor, so, if 2 sensors, about 4.6 seconds between reads for a 2 second interval.
#define _pin1Dir DIR_X8  //Netcruzer Alias for Pin Direction (DIR_xx)
#define _pin1In PIN_X8   //Netcruzer Alias for Pin Read (PIN_xx)
#define _pin1Out LAT_X8  //Netcruzer Alias for Pin Write (LAT_xx)
#define _pin2Dir DIR_X7  //Netcruzer Alias for Pin Direction (DIR_xx)
#define _pin2In PIN_X7   //Netcruzer Alias for Pin Read (PIN_xx)
#define _pin2Out LAT_X7  //Netcruzer Alias for Pin Write (LAT_xx)
#define _pin3Dir DIR_X6  //Netcruzer Alias for Pin Direction (DIR_xx)
#define _pin3In PIN_X6   //Netcruzer Alias for Pin Read (PIN_xx)
#define _pin3Out LAT_X6  //Netcruzer Alias for Pin Write (LAT_xx)
#define _pin4Dir DIR_X4  //Netcruzer Alias for Pin Direction (DIR_xx)
#define _pin4In PIN_X4   //Netcruzer Alias for Pin Read (PIN_xx)
#define _pin4Out LAT_X4  //Netcruzer Alias for Pin Write (LAT_xx)
#define _numSensors 4

//extern BYTE sensor;

    static enum _DHTState
    {
        S_START,
        S_INIT,
        S_READ,
    } DHTState;

  void DHT_init();
  void DHT_run();
  WORD DHT_readT(int k);
  WORD DHT_readH(int k);
  BOOL DHT_health(int k);
  

#endif
