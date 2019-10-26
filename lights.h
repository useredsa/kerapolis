#ifndef KERAPOLIS_LIGHTS_H
#define KERAPOLIS_LIGHT_H

#include <stdint.h>

const uint8_t lightGraphic[48] =
{    60,   60,   60,   60,   60,   60,   60,   60,   60,   60,   60,   60,   60,   60,  100,  100,  140,  140,  180,  180,    0,    0,    0,    0,
// 00:0, 00:3, 01:0, 01:3, 02:0, 02:3, 03:0, 03:3, 04:0, 04:3, 05:0, 05:3, 06:0, 06:3, 07:0, 07:3, 08:0, 08:3, 09:0, 09:3, 10:0, 10:3, 11:0, 11:3,
      0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  180,  180,  180,  180,  150,  150,  150,  150,  120,   60,   60 };
// 12:0, 12:3, 13:0, 13:3, 14:0, 14:3, 15:0, 15:3, 16:0, 16:3, 17:0, 17:3, 18:0, 18:3, 19:0, 19:3, 20:0, 20:3, 21:0, 21:3, 22:0, 22:3, 23:0, 23:3

const uint8_t streetLights[18] = {2, 2, 3, 3, 2, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3};

#define RX_KERALIGHTS 36
#define TX_KERALIGHTS 4
#define RE_KERALIGHTS 17
#define DE_KERALIGHTS 14

void lightsTest();

/**
 * @brief Initialization of serial port and light drivers.
 */
void initLights();

/**
 * @brief Set the light intensity according to current time and events.
 * Gets time from cityStatus.
 * Accesses current events.
 */
void setLightIntensity();

/**
 * @brief Check the integrity of the light system
 * Updates broken lights in cityStatus.
 * Updates consum in cityStatus.
 */
void checkLightStatus();

/**
 * @brief Checks if an event is taken place and disables that street.
 */
void manageEvents();

#endif
