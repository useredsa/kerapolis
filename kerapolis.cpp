#define TELL

#include "JSONParser.h"
#include "comm.h"
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

void loop(){

  if (connected) {
    int pkgSize = udp.parsePacket();
    if (pkgSize) {
      if (udp.remotePort() == 64524) {
        Serial.println("Received packet");
        int len = udp.read(pkgBuff, pkgBuffSize);
        if (len > 0) pkgBuff[len] = 0;
        Serial.println(pkgBuff);
        parseInfo(pkgBuff);
        setLightIntensity();
        sendCityStatus();
      } else {
        Serial.print("Received packet from ");
        Serial.println(udp.remotePort());
        udp.read(pkgBuff, pkgBuffSize);
      }
    }
  }
  delay(1000);
}
