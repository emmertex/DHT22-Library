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

BYTE data[6];
WORD checksum;
BYTE goodData[4][_numSensors];
DWORD lastTime;
BOOL health[_numSensors];
BYTE sensor = 0;

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
                }
                lastTime = getTick16bit_1ms();
                DHTState = S_READ;
            }
            break;
            

        case S_READ:
            if ((getTick16bit_1ms() - lastTime) >= 20) {
                j = 1;
                switch (sensor) {
                    case 1:
                        _pin1Out = 1;
                        break;

                    case 2:
                        _pin2Out = 1;
                        break;
                }
                delay_us(1);
                switch (sensor) {
                    case 1:
                        _pin1Dir = INPUT_PIN;
                        break;

                    case 2:
                        _pin2Dir = INPUT_PIN;
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
                }


                    if ((i % 2 == 0)) {
                        data[j / 8] <<= 1;
                        if (counter > _count) data[j / 8] |= 1;
                        j++;
                    }

                }

                //Check Checksum
                checksum = data[0]+data[1]+data[2]+data[3];
                if ((data[4] == (checksum & 255)) || (checksum == 0) || (checksum == 255)) {
                    BYTE i;
                    for (i=0;i<4;i++)
                    {
                        goodData[i][sensor-1] = data[i];
                    }
                    health[sensor-1] = 0;
                } else {
                    health[sensor-1] = 1;
                }

                lastTime = getTick16bit_1ms();
                DHTState = S_START;
            }
            break;
            
    }
    return;
}


WORD DHT_readT(int i) {
      return ((goodData[2][i] * 256) + goodData[3][i]);
}

WORD DHT_readH(int i) {
      return ((goodData[0][i] * 256) + goodData[1][i]);
}

BOOL DHT_health(int i) {
    return health[i];
}
