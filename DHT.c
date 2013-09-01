/*
 * DHT22 Library for Netcruzer (MODTRONIX) by Andrew Frahn (EMMERTEX)
 * emmertex@gmail.com
*/

#include <HardwareProfile.h>
#include "DHT.h"
#include "nz_tick.h"
#include "nz_delay.h"

BYTE data[6];
WORD checksum;
BYTE goodData[4][2];
DWORD lastTime;
BOOL health[2];
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
                    case 0:
                        _pin1Dir = OUTPUT_PIN;
                        _pin1Out = 1;

                    case 1:
                        _pin2Dir = OUTPUT_PIN;
                        _pin2Out = 1;
                }
                lastTime = getTick16bit_1ms();
                DHTState = S_INIT;
            }
            break;
           


        case S_INIT:
            if ((getTick16bit_1ms() - lastTime) >= 250) {
                data[0] = data[1] = data[2] = data[3] = data[4] = data[5] = 0;
                switch (sensor) {
                    case 0:
                        _pin1Dir = OUTPUT_PIN;
                        _pin1Out = 0;

                    case 1:
                        _pin2Dir = OUTPUT_PIN;
                        _pin2Out = 0;
                }
                lastTime = getTick16bit_1ms();
                DHTState = S_READ;
            }
            break;
            

        case S_READ:
            if ((getTick16bit_1ms() - lastTime) >= 20) {
                j = 1;
                switch (sensor) {
                    case 0:
                        _pin1Out = 1;

                    case 1:
                        _pin2Out = 1;
                }
                delay_us(1);
                switch (sensor) {
                    case 0:
                        _pin1Dir = INPUT_PIN;

                    case 1:
                        _pin2Dir = INPUT_PIN;
                }


                for (i = 0; i < _timings; i++) {
                    counter = 0;
                switch (sensor) {
                    case 0:
                        while (_pin1In == laststate) {
                            counter++;
                            if (counter == _maxCount) break;
                        }
                        laststate = _pin1In;

                    case 1:
                        while (_pin2In == laststate) {
                            counter++;
                            if (counter == _maxCount) break;
                        }
                        laststate = _pin2In;
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
                        goodData[i][sensor] = data[i];
                    }
                    health[sensor] = 0;
                } else {
                    health[sensor] = 1;
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
