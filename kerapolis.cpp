#define TELL

#include "JSONParser.h"
#include "comm.h"
#include "Arduino.h"
#include "lights.h"

void setup(){
  // Initilize hardware serial:
  Serial.begin(115200);
  delay(100);
  // Connect to the WiFi network
  connectToWiFi(networkName, networkPswd);
  initLights();
}

const int pkgBuffSize = 500;
char pkgBuff[pkgBuffSize];

bool isInfo(char *buf) {
  static char *str = "{\"KERAPolisInfo";
  for (int i = 0; i < 14; i++) {
    if (str[i] != pkgBuff[i])
      return false;
  }
  return true;
}

bool isEvent(char *buf) {
  static char *str = "{\"KERAPolisEvent";
  for (int i = 0; i < 14; i++) {
    if (str[i] != pkgBuff[i])
      return false;
  }
  return true;
}

bool isStatus(char *buf) {
  static char *str = "{\"KERAPolisStatus";
  for (int i = 0; i < 15; i++) {
    if (str[i] != pkgBuff[i])
      return false;
  }
  return true;
}

const int updateLapsus = 1000;
int lastUpdate = 0;

void loop(){
  if (connected) {
    int pkgSize = udp.parsePacket();
    if (pkgSize) {
      //Serial.println("Received packet");
      int len = udp.read(pkgBuff, pkgBuffSize);
      if (len > 0) pkgBuff[len] = 0;
      //Serial.println(pkgBuff);
      if (isInfo(pkgBuff)) {
        parseInfo(pkgBuff);
      } else if (isEvent(pkgBuff)) {
        parseEvent(pkgBuff);
      } else if (!isStatus(pkgBuff)) {
        Serial.print("received smt else: ");
        pkgBuff[20] = 0;
        Serial.println(pkgBuff);
      }
    }
  }

  //checkLightStatus();
  manageEvents();
  setLightIntensity();
  
  int mil = millis();
  if (lastUpdate+updateLapsus < mil) {
    lastUpdate = mil;
    cityStatus.frameTime = cityInfo.localTime();
    sendCityStatus();
    Serial.print("Time: ");
    Serial.print(cityInfo.time.toString());
    Serial.print("  Status ");
    Serial.print(cityStatus.frameTime.toString());
    Serial.print(" Gasto(A) ");
    Serial.print(cityStatus.lightsTotalConsum);
    Serial.print(" Consum ");
    Serial.println(cityStatus.lightsTotalCost);
  }
}
