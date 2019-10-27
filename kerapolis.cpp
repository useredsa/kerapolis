#define TELL

#include "JSONParser.h"
#include "comm.h"
#include "Arduino.h"
#include "lights.h"
#include "ESP32_Servo.h"
#include "PID_v1.h"

void writeEthernet(uint8_t cmd, uint8_t param);

Servo farmWindow;
double input, output, setpoint;
PID pressureControl(&input, &output, &setpoint, 0, 0, 0, DIRECT);
uint8_t ethData[5];
int32_t pump1, pump2, pressure1, pressure2; // pump 2 is tank pump
uint8_t ev1Status = 0;

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
  // Asynchronous ethernet read init
  if(ethIn.listen(54322)) {
    ethIn.onPacket([](AsyncUDPPacket packet) {
      pump1 = packet.data()[3];
      pump2 = packet.data()[11];
      pressure1 = ((int32_t)packet.data()[5] << 16)+((int32_t)packet.data()[6] << 8) + (int32_t)packet.data()[7];
      pressure2 = ((int32_t)packet.data()[13] << 16)+((int32_t)packet.data()[14] << 8) + (int32_t)packet.data()[15];
      if(pressure2 > 30000) pressure2 = 0;
      cityStatus.waterTankLevel = pressure2/170;
      cityStatus.waterPumpPercentage = pressure1/1000;
      cityStatus.waterPressure = pressure2/170;
      Serial.print("pump1:");
      Serial.print(pump1);
      Serial.print("pressure1:");
      Serial.print(pressure1);
      Serial.print("\tPressure2:");
      Serial.println(pressure2);
      /*for(int i = 0; i < 16; i++){
        Serial.print(packet.data()[i]);
        Serial.print("\t");
      }
      Serial.println();*/
      Serial.write(packet.data(), packet.length());
    });
  }
  writeEthernet(3, 30);
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

  if(cityInfo.humidity < 65 && ev1Status == 0){
    writeEthernet(1, 50);
    Serial.println("Watering");
    cityStatus.farmWatering = 1;
    ev1Status = 1;
  }
  else if(cityInfo.humidity >= 65 && ev1Status == 1){
    writeEthernet(2, 50);
    Serial.println("Finished watering");
    cityStatus.farmWatering = 0;
    ev1Status = 0;
  }

  if(pressure2 < 4000 && pump2 != 50){
    writeEthernet(4, 50);
  }
  else if(pressure2 > 14000 && pump2 != 0){
    writeEthernet(4, 0);
  }


  pressureControl.Compute();
  writeEthernet(3, setpoint);
}

void writeEthernet(uint8_t cmd, uint8_t param){
  ethData[0] = cmd;
  for(int i = 1; i < 4; i++) ethData[i] = 0;
  ethData[4] = param;
  ethOut.writeTo(ethData, 5, IPAddress(192,168,0,81), 1028);
}
