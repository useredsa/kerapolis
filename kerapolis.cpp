#include "comm.h"
#include "TCityJSONParser.h"

void setup(){
  // Initilize hardware serial:
  Serial.begin(115200);
  delay(10);

  //Connect to the WiFi network
  connectToWiFi(networkName, networkPswd);
}

const int pkgBuffSize = 500;
char pkgBuff[pkgBuffSize];
TCityJSONParser parser;

void loop(){
//  //only send data when connected
//  if(connected){
//    //Send a packet
//    udp.beginPacket(udpAddress,udpPort);
//    udp.printf("Seconds since boot: %lu", millis()/1000);
//    udp.endPacket();
//  }
//  //Wait for 1 second
//  delay(1000);

  if (connected) {
    int pkgSize = udp.parsePacket();
    if (pkgSize) {
      if (udp.remotePort() == 54336) {
        Serial.println("Received packet");
        int len = udp.read(pkgBuff, pkgBuffSize);
        if (len > 0) pkgBuff[len] = 0;
        Serial.println(pkgBuff);
        parser.getCityInfoFromNetwork(pkgBuff);
      } else {
        Serial.print("Received packet from ");
        Serial.println(udp.remotePort());
        udp.read(pkgBuff, pkgBuffSize);
      }
    }
  }
  //delay(10);
}
