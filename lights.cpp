#include "lights.h"

#include "JSONParser.h"
#include <Arduino.h>
#include <HardwareSerial.h>

/*
 * Serial Communication variables and Macros
 */

HardwareSerial ledSerial(1);
uint8_t out[5];   //Revision: Could use the stack instead of global array
uint8_t in[8];
uint16_t crc16(uint8_t *nData, uint16_t wLength);


#define RX_KERALIGHTS 36
#define TX_KERALIGHTS 4
#define RE_KERALIGHTS 17
#define DE_KERALIGHTS 14

#define ALL     0x00
#define ENABLE  0x04
#define QUERY   0x02
#define TURNON  0x08
#define TURNOFF 0x09
#define SETPOW  0x07

#define SEND(who, opcode)         \
  do {                            \
    out[0] = (uint8_t) who;       \
    out[1] = (uint8_t) opcode;    \
    int crc = crc16(out, 2);      \
    out[2] = (uint8_t) crc;       \
    out[3] = (uint8_t) (crc>>8);  \
    ledSerial.write(out, 4);      \
    ledSerial.flush();            \
  } while (0)

#define SENDP(who, opcode, param) \
  do {                            \
    out[0] = (uint8_t) who;       \
    out[1] = (uint8_t) opcode;    \
    out[2] = (uint8_t) param;     \
    int crc = crc16(out, 3);      \
    out[3] = (uint8_t) crc;       \
    out[4] = (uint8_t) (crc>>8);  \
    ledSerial.write(out, 5);      \
    ledSerial.flush();            \
  } while (0)


const int numTotalLights = 40;
bool enabled[numLights];
uint8_t lightIntensity0[numLights];
uint8_t lightIntensity60[numLights];
uint8_t lightIntensity100[numLights];
uint8_t lightIntensity120[numLights];
uint8_t lightIntensity140[numLights];
uint8_t lightIntensity150[numLights];
uint8_t lightIntensity180[numLights];
int lightPower = 0;

void initLights(){
  // Set serial pins to output mode
  pinMode(RE_KERALIGHTS, OUTPUT);
  pinMode(DE_KERALIGHTS, OUTPUT);
  // Initialize pins to write
  digitalWrite(RE_KERALIGHTS, HIGH);
  digitalWrite(DE_KERALIGHTS, HIGH);
  // Initialize connection
  ledSerial.begin(115200, SERIAL_8E1, RX_KERALIGHTS, TX_KERALIGHTS);
  // Enable the lights
  SENDP(ALL, ENABLE, 0x01);
  for (int i = 0; i < numLights; i++)
    enabled[i] = true;
}

/**
 * @brief small function to test, not for use
 */
void lightsTest(){
  SENDP(0x0, SETPOW, 0x40);
  digitalWrite(RE_KERALIGHTS, HIGH);
  digitalWrite(DE_KERALIGHTS, HIGH);
  SEND(0x0, TURNON);
  while(1){
    digitalWrite(RE_KERALIGHTS, HIGH);
    digitalWrite(DE_KERALIGHTS, HIGH);
    SEND(0x2, QUERY);
    Serial.println("Sending:");
    for(int i = 0; i < 4; i++){
      Serial.print(out[i]);
      Serial.print("\t");
    }
    Serial.println();
    digitalWrite(RE_KERALIGHTS, LOW);
    digitalWrite(DE_KERALIGHTS, LOW);
    while(!ledSerial.available());
    Serial.println("Receiving:");
    while(ledSerial.available()){
      Serial.print((uint8_t)ledSerial.read());
      Serial.print("\t");
    }
    Serial.println();
    delay(1000);
  }
}

void setLightIntensity() {
  int i = cityInfo.timeZone();
  // Only update on change:
  if (lightPower != lightGraphic[i]) {
    // Set pins to write
    digitalWrite(RE_KERALIGHTS, HIGH);
    digitalWrite(DE_KERALIGHTS, HIGH);
    // Save energy turning off the lights
    if (lightGraphic[i] == 0) {
      SEND(ALL, TURNOFF);
    } else {
      SENDP(ALL, SETPOW, lightGraphic[i]);
      SEND(ALL, TURNON);
    }
    lightPower = lightGraphic[i];
  }
}

void checkLightStatus() {
  int totalBrokenLights = 0;
  int power = lightGraphic[cityInfo.timeZone()];
  float consum = 0.0;
  // Selects the table to look at
  uint8_t * lightIntensity;
  switch(power) {
    default:
    case 0: lightIntensity = lightIntensity0; break;
    case 60 : lightIntensity = lightIntensity60; break;
    case 100 : lightIntensity = lightIntensity100; break;
    case 120 : lightIntensity = lightIntensity120; break;
    case 140 : lightIntensity = lightIntensity140; break;
    case 150 : lightIntensity = lightIntensity150; break;
    case 180 : lightIntensity = lightIntensity180; break;
  }

  // Clear the list of broken lights to update it now
  // Alternatively, we could do a scan on broken lights to check if they
  // are fixed
  cityStatus.lightsBrokenList.clear();

  // Checks that the readings of every street match the ones recorded in the array
  for (int i = 0; i < numLights; i++) {
    if (!enabled[i]) continue;
    // Query state
    digitalWrite(RE_KERALIGHTS, HIGH);
    digitalWrite(DE_KERALIGHTS, HIGH);
    SEND((i+1), QUERY);

    // Read result
    digitalWrite(RE_KERALIGHTS, LOW);
    digitalWrite(DE_KERALIGHTS, LOW);
    long t = millis();
    while(!ledSerial.available() && t + 5 > millis()); // harcoded timeout of 5 ms
    if(t + 5 > millis()){
      while(ledSerial.available()) ledSerial.readBytes(in, 8);
      uint16_t intensity = (uint16_t)in[5] + ((uint16_t)in[4] << 8);
      // Get intensity, update consum and check if its value is normal
      consum += (float)intensity;
      // Calculates the number of broken lamps
      // Hardcoding values! :D
      int brokenLights = 0;
      if(streetLights[i] == 3 && (uint16_t)(lightIntensity[i] * 0.85) > intensity) brokenLights++;
      if(streetLights[i] == 3 && (uint16_t)(lightIntensity[i] * 0.55) > intensity) brokenLights++;
      if(streetLights[i] == 2 && (uint16_t)(lightIntensity[i] * 0.6) > intensity) brokenLights++;
      if((uint16_t)(lightIntensity[i] * 0.15) > intensity) brokenLights++;

      // If the any light is broken update info
      // Otherwise update the table or normal intensity
      totalBrokenLights += brokenLights;
      if(brokenLights > 0) cityStatus.lightsBrokenList.push_back(i+1);
      else lightIntensity[i] = intensity;
    } else {
      totalBrokenLights += streetLights[i];
      cityStatus.lightsBrokenList.push_back(i+1);
    }
    delay(5);
  }
  digitalWrite(RE_KERALIGHTS, HIGH);
  digitalWrite(DE_KERALIGHTS, HIGH);

  cityStatus.lightsTotalConsum = consum;
  cityStatus.lightsTotalCost = 24 * consum * cityInfo.lightPrice;
  cityStatus.numLightsOk = numTotalLights - totalBrokenLights;
  cityStatus.numLightsBroken = totalBrokenLights;
}

void manageEvents() {
  for (int i = 0; i < maxNumEvents; i++) {
    if (cityEvents[i].status == CityEvent::Status::NOT_STARTED) {
      if (cityInfo.localTime().between(cityEvents[i].startTime, cityEvents[i].endTime)) {
        cityEvents[i].status = CityEvent::Status::STARTED;
        SEND(cityEvents[i].zone, TURNOFF);
        SENDP(cityEvents[i].zone, ENABLE, 0x00);
        enabled[cityEvents[i].zone] = false;
        Serial.println("Event started!!");
      }
    } else if (cityEvents[i].status == CityEvent::Status::STARTED) {
      if (!cityInfo.localTime().between(cityEvents[i].startTime, cityEvents[i].endTime)) {
        cityEvents[i].status = CityEvent::Status::UNSET;
        SENDP(cityEvents[i].zone, ENABLE, 0x01);
        SENDP(cityEvents[i].zone, SETPOW, lightGraphic[cityInfo.timeZone()]);
        SEND(cityEvents[i].zone, TURNON);
        enabled[cityEvents[i].zone] = true;
        Serial.println("Event finished :(");
      }
    }
  }
}


uint16_t crc16(uint8_t *nData, uint16_t wLength){
  static uint16_t wCRCTable[] = {
  0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
  0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
  0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
  0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
  0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
  0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
  0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
  0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
  0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
  0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
  0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
  0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
  0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
  0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
  0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
  0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
  0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
  0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
  0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
  0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
  0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
  0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
  0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
  0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
  0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
  0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
  0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
  0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
  0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
  0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
  0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
  0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040 };

  uint8_t nTemp;
  uint16_t wCRCWord = 0xFFFF;

  while (wLength--)
  {
    nTemp = *nData++ ^ wCRCWord;
    wCRCWord >>= 8;
    wCRCWord ^= wCRCTable[nTemp];
  }
  return wCRCWord;
}
