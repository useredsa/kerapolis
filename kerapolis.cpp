#define TELL

#include "JSONParser.h"
#include "comm.h"
#include "Arduino.h"
#include "lights.h"
#include "ESP32_Servo.h"
#include "PID_v1.h"

#define WRITE_ETHERNET(cmd, param) 0
#define READ_ETHERNET(N) N

Servo farmWindow;
double input, output, setpoint;
PID pressureControl(&input, &output, &setpoint, 0, 0, 0, DIRECT);

void setup(){
  // Initilize hardware serial:
  Serial.begin(115200);
  delay(100);
  // Connect to the WiFi network
  connectToWiFi(networkName, networkPswd);
  cityStatus.lightRunning = true;
  cityStatus.farmRunning = true;
  cityStatus.waterRunning = true;
  initLights();
  // Connect farm servo
  farmWindow.attach(13);
  farmWindow.write(0);
  cityStatus.farmRoofPosition = 0;
  //PID Initialization
  input = 0;
  setpoint = 50;
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
const int lightCheckLapsus = 2500;
int lastCheck = 0;

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

  manageEvents();
  setLightIntensity();

  int mil = millis();
  if (lastCheck+lightCheckLapsus < mil) {
    lastCheck = mil;
    checkLightStatus();
    ethUDP.writeTo((uint8_t*) "Anyone here?", 6, IPAddress(192,168,0,81), 1024);
  }
  if (lastUpdate+updateLapsus < mil) {
    lastUpdate = mil;
    Serial.print("Time: ");
    Serial.println(cityInfo.localTime().toString());
    cityStatus.frameTime = cityInfo.localTime();
    sendCityStatus();
  }

  if(cityInfo.temperature > 27 && farmWindow.read() < 10){
    farmWindow.write(180);
    cityStatus.farmRoofPosition = 0;
  }
  else if(cityInfo.temperature <= 27 && farmWindow.read() > 170){
    farmWindow.write(0);
    cityStatus.farmRoofPosition = 2;
  }

  if(cityInfo.humidity < 50 && READ_ETHERNET(0) < 10){
    WRITE_ETHERNET(1, 1);
    WRITE_ETHERNET(2, 1);
    cityStatus.farmWatering = 1;
  }
  else if(cityInfo.humidity >= 50 && READ_ETHERNET(50) > 10){
    WRITE_ETHERNET(1, 0);
    WRITE_ETHERNET(2, 0);
    cityStatus.farmWatering = 0;
  }
}
