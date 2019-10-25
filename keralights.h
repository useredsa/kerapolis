#ifndef __KERALIGHTS__
#define __KERALIGHTS__

#include <stdint.h>

#define RX_KERALIGHTS 36
#define TX_KERALIGHTS 4
#define RE_KERALIGHTS 17
#define DE_KERALIGHTS 14

// Initialization of serial port and light drivers
void initLights();

void turnOnBroadcast(uint8_t intensity);
void turnOffBroadcast();

uint8_t turnOn(uint8_t intensity, uint8_t lightID);
void turnOff(uint8_t lightID);

// gets the intensity of the current light, 0 if no response is received
uint16_t getIntensity(uint8_t lightID);

#endif
