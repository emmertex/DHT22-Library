/*
 * DHT22 Library for Netcruzer (MODTRONIX) by Andrew Frahn (EMMERTEX)
 * emmertex@gmail.com
*/

#include <HardwareProfile.h>
#include "DHT.h"
#include "nz_tick.h"
#include "nz_delay.h"

BYTE data[6];
BYTE goodData[4];
DWORD lastTime;
BOOL health = 1;

void initDHT() {
    DHTState = S_START;
    lastTime = getTick16bit_1ms();
}

void DHTTask() {
    BYTE laststate = 1;
    BYTE counter = 0;
    BYTE j = 0, i;
    switch (DHTState) {
        case S_START:
            if ((getTick16bit_1ms() - lastTime) >= _readInterval) {
                _pinDir = OUTPUT_PIN;
                _pinOut = 1;
                lastTime = getTick16bit_1ms();
                DHTState = S_INIT;
            }
            break;
           


        case S_INIT:
            if ((getTick16bit_1ms() - lastTime) >= 250) {
                data[0] = data[1] = data[2] = data[3] = data[4] = data[5] = 0;
                _pinDir = OUTPUT_PIN;
                _pinOut = 0;
                lastTime = getTick16bit_1ms();
                DHTState = S_READ;
            }
            break;
            

        case S_READ:
            if ((getTick16bit_1ms() - lastTime) >= 20) {
                j = 1;
                _pinOut = 1;
                delay_us(1);
                _pinDir = INPUT_PIN;


                for (i = 0; i < _timings; i++) {
                    counter = 0;
                    while (_pinIn == laststate) {
                        counter++;
                        
                        if (counter == _maxCount) {
                            break;
                       
                        }
                    if (counter == _maxCount) break;
                    }
                    laststate = _pinIn;


                    if ((i % 2 == 0)) {
                        data[j / 8] <<= 1;
                        if (counter > _count)
                            data[j / 8] |= 1;
                        j++;
                    }

                }

                //Check Checksum
                if (((data[0]+data[1]+data[2]+data[3]) & 255) == data[4]) {
                    goodData[0] = data[0];
                    goodData[1] = data[1];
                    goodData[2] = data[2];
                    goodData[3] = data[3];
                    health = 0;
                } else {
                    health = 1;
                }

                lastTime = getTick16bit_1ms();
                DHTState = S_START;
            }
            break;
            
    }
    return;
}


WORD DHTTemperature() {
      return ((goodData[2] * 256) + goodData[3]);
}

WORD DHTHumidity() {
      return ((goodData[0] * 256) + goodData[1]);
}

BOOL DHTHealth() {
    return health;
}
