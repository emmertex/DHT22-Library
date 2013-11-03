/*
 * DHT22 Library for Netcruzer (MODTRONIX) by Andrew Frahn (EMMERTEX)
 
 Copyright 2013 - Andrew Frahn (Emmertex)
  emmertex@gmail.com
 
 This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Australia License.
 To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/au/.
 
*/

#include <HardwareProfile.h>
#include "DHT.h"
#include "nz_tick.h"
#include "nz_delay.h"
#include "C:\Users\tech-pc\Documents\GitHub\Modbus-Library\Modbus.h"

    #define MY_DEBUG_LEVEL  DEBUG_LEVEL_INFO    //Set debug level. All debug messages with equal or higher priority will be outputted

#include "nz_debug.h"                           //Required for debugging. This include MUST be after "#define MY_DEBUG_LEVEL ..."!


BYTE data[6];
BYTE checksum;
BYTE goodData[4][_numSensors];
DWORD lastTime;
BOOL health[_numSensors];
BYTE sensor = 1;

void DHT_init() {
    health[0] = health[1] = 1;
    DHTState = S_START;
    lastTime = getTick16bit_1ms();
}

void DHT_run() {
    BYTE laststate = 1;
    BYTE counter = 0;
    BYTE j = 0, i;
    switch (DHTState) {
        case S_START:
            if ((getTick16bit_1ms() - lastTime) >= _readInterval) {
                sensor = 1 + sensor * (sensor < _numSensors);
                switch (sensor) {
                    case 1:
                        _pin1Dir = OUTPUT_PIN;
                        _pin1Out = 1;
                        break;
                    case 2:
                        _pin2Dir = OUTPUT_PIN;
                        _pin2Out = 1;
                        break;                    
                    case 3:
                        _pin3Dir = OUTPUT_PIN;
                        _pin3Out = 1;
                        break;                    
                    case 4:
                        _pin4Dir = OUTPUT_PIN;
                        _pin4Out = 1;
                        break;
                }
                lastTime = getTick16bit_1ms();
                DHTState = S_INIT;
            }
            break;
           


        case S_INIT:
            if ((getTick16bit_1ms() - lastTime) >= 250) {
                data[0] = data[1] = data[2] = data[3] = data[4] = data[5] = 0;
                switch (sensor) {
                    case 1:
                        _pin1Dir = OUTPUT_PIN;
                        _pin1Out = 0;
                        break;

                    case 2:
                        _pin2Dir = OUTPUT_PIN;
                        _pin2Out = 0;
                        break;
                    case 3:
                        _pin3Dir = OUTPUT_PIN;
                        _pin3Out = 0;
                        break;

                    case 4:
                        _pin4Dir = OUTPUT_PIN;
                        _pin4Out = 0;
                        break;
                }
                lastTime = getTick16bit_1ms();
                DHTState = S_READ;
            }
            break;
            

        case S_READ:
            if ((getTick16bit_1ms() - lastTime) >= 20) {
                j = 3;
                switch (sensor) {
                    case 1:
                        _pin1Out = 1;
                        break;

                    case 2:
                        _pin2Out = 1;
                        break;
                   
                   case 3:
                        _pin3Out = 1;
                        break;

                    case 4:
                        _pin4Out = 1;
                        break;
                }
                delay_us(50);
                switch (sensor) {
                    case 1:
                        _pin1Dir = INPUT_PIN;
                        break;

                    case 2:
                        _pin2Dir = INPUT_PIN;
                        break;
                    case 3:
                        _pin3Dir = INPUT_PIN;
                        break;

                    case 4:
                        _pin4Dir = INPUT_PIN;
                        break;
                }

                
                
                for (i = 0; i < _timings; i++) {
                    counter = 0;
                switch (sensor) {
                    case 1:
                        while (_pin1In == laststate) {
                            counter++;
                            if (counter == _maxCount) break;
                        }
                        laststate = _pin1In;
                        break;

                    case 2:
                        while (_pin2In == laststate) {
                            counter++;
                            if (counter == _maxCount) break;
                        }
                        laststate = _pin2In;
                        break;
                    case 3:
                        while (_pin3In == laststate) {
                            counter++;
                            if (counter == _maxCount) break;
                        }
                        laststate = _pin3In;
                        break;

                    case 4:
                        while (_pin4In == laststate) {
                            counter++;
                            if (counter == _maxCount) break;
                        }
                        laststate = _pin4In;
                        break;
                }


                    if ((i % 2 == 0)) {
                        
                        data[j / 8] <<= 1;
                        if (counter > _count) data[j / 8] |= 1;
                        j++;
                        
                    }

                }

                //Check Checksum
                checksum = (data[0]+data[1]+data[2]+data[3])&0xFF;

                if ((data[4] == checksum) || (checksum == 0) || (checksum == 255)) {
                    DEBUG_PUT_STR(DEBUG_LEVEL_INFO, "\nSensor ");
                    DEBUG_PUT_WORD(DEBUG_LEVEL_INFO, (sensor-1));
                    DEBUG_PUT_STR(DEBUG_LEVEL_INFO, ": ");

                    BYTE i;
                    for (i=0;i<4;i++)
                    {
                        goodData[i][sensor-1] = data[i];
                        DEBUG_PUT_STR(DEBUG_LEVEL_INFO, "   ");
                        DEBUG_PUT_WORD(DEBUG_LEVEL_INFO, goodData[i][sensor-1]);
                    }
                    health[sensor-1] = 1;
                } else {
                    health[sensor-1] = 0;
                }
/*
                BYTE m,n;
                for (m=0;m<4;m++){
                    DEBUG_PUT_STR(DEBUG_LEVEL_INFO, "\nLine ");
                    DEBUG_PUT_WORD(DEBUG_LEVEL_INFO, m);
                    DEBUG_PUT_STR(DEBUG_LEVEL_INFO, ": ");
                    for (n=0;n<4;n++){
                        DEBUG_PUT_STR(DEBUG_LEVEL_INFO, " ");
                        DEBUG_PUT_WORD(DEBUG_LEVEL_INFO, goodData[m][n]);

                    }
                }*/

                lastTime = getTick16bit_1ms();
                DHTState = S_START;
            }
            break;
            
    }
    return;
}


WORD DHT_readT(int k) {
      return ((goodData[2][k] * 256) + goodData[3][k]);
}

WORD DHT_readH(int k) {
      return ((goodData[0][k] * 256) + goodData[1][k]);
}

BOOL DHT_health(int k) {
    return health[k];
}
