#include "JSONParser.h"

#include <ArduinoJson.h>
#include <string>

void printCityInfo();
void printCityStatus();

void parseInfo(char *inputData) {
  ArduinoJson::StaticJsonDocument<250> doc;
  ArduinoJson::DeserializationError error = ArduinoJson::deserializeJson(doc, (char *)inputData);
	if (error == ArduinoJson::DeserializationError::Ok) {
		cityInfo.time.fromString(doc["KERAPolisInfo"]["time"]);
		cityInfo.lightPrice = doc["KERAPolisInfo"]["lightPrice"];
		cityInfo.humidity = doc["KERAPolisInfo"]["humidity"];
		cityInfo.temperature = doc["KERAPolisInfo"]["temperature"];
    //printCityInfo();
	}
  // Problem parsing!
}

void parseEvent(char *inputData) {
  ArduinoJson::StaticJsonDocument<250> doc;
  ArduinoJson::DeserializationError error = ArduinoJson::deserializeJson(doc, (char *)inputData);
  if (error == ArduinoJson::DeserializationError::Ok) {
    for (int i = 0; i < maxNumEvents; i++) {
      if (cityEvents[i].status == CityEvent::Status::UNSET) {
        cityEvents[i].status = CityEvent::Status::NOT_STARTED;
        cityEvents[i].description = "streetLightsOff"; //TODO hardcoded
        cityEvents[i].zone = doc["KERAPolisEvent"]["zone"];
        cityEvents[i].receivedTime = cityInfo.localTime();
        cityEvents[i].startTime.fromString(doc["KERAPolisEvent"]["startTime"]);
        cityEvents[i].endTime.fromString(doc["KERAPolisEvent"]["endTime"]);
        Serial.print("Received EVENT!!!: zone ");
        Serial.print(cityEvents[i].zone);
        Serial.print(" times[] ");
        Serial.print(cityEvents[i].startTime.toString());
        Serial.print(" ");
        Serial.println(cityEvents[i].endTime.toString());
        return;
      }
    }
    Serial.println("Event couldn't be parsed because there were too many events");
  }
  // Problem parsing!
}


std::string encodeStatus() {
	char statusJSON[800];
	ArduinoJson::StaticJsonDocument<800> doc;
	doc["KERAPolisStatus"]["time"] = cityStatus.frameTime.toString();
	doc["KERAPolisStatus"]["farmRunning"] = cityStatus.farmRunning;
	doc["KERAPolisStatus"]["waterRunning"] = cityStatus.waterRunning;
	doc["KERAPolisStatus"]["lightRunning"] = cityStatus.lightRunning;
  doc["KERAPolisStatus"]["numLightsOk"] = cityStatus.numLightsOk;
  doc["KERAPolisStatus"]["numLightsBroken"] = cityStatus.numLightsBroken;
  static char brokenList[50];
  char* pos = brokenList;
  *(pos++) = '[';
  for (int i = 0; i < (int) cityStatus.lightsBrokenList.size(); i++) {
    if (i) *(pos++) = ',';
    itoa(cityStatus.lightsBrokenList[i], pos, 10);
    if (cityStatus.lightsBrokenList[i] < 10)
      pos++;
    else
      pos+=2;
  }
  *(pos++) = ']';
  *(pos++) = '\0';
  doc["KERAPolisStatus"]["lightsBrokenList"] = brokenList;
  doc["KERAPolisStatus"]["lightsTotalConsum"] = cityStatus.lightsTotalConsum;
  doc["KERAPolisStatus"]["lightsTotalCost"] = cityStatus.lightsTotalCost;
  doc["KERAPolisStatus"]["farmWatering"] = cityStatus.farmWatering;
  doc["KERAPolisStatus"]["farmRoofPosition"] = cityStatus.farmRoofPosition;
  doc["KERAPolisStatus"]["farmTempOk"] = cityStatus.farmTempOk;
  doc["KERAPolisStatus"]["waterOk"] = cityStatus.waterOk;
  doc["KERAPolisStatus"]["waterTankLevel"] = cityStatus.waterTankLevel;
  doc["KERAPolisStatus"]["waterPressure"] = cityStatus.waterPressure;
  doc["KERAPolisStatus"]["waterPumpPercentage"] = cityStatus.waterPumpPercentage;
	ArduinoJson::serializeJson(doc, statusJSON);
  //printCityStatus();
	return std::string(statusJSON);
}

void printCityInfo() {
    Serial.print("KERAPolis Info -->");
    Serial.print("[time:");
    Serial.print(cityInfo.time.toString());
    Serial.print("][humidity:");
    Serial.print(cityInfo.humidity);
    Serial.print("][temperature:");
    Serial.print(cityInfo.temperature);
    Serial.print("]");
    Serial.println();
}

void printCityStatus() {
    Serial.print("KERAPolis STATUS --> ");
    Serial.print("[time:");
    Serial.print(cityStatus.frameTime.toString());
    Serial.print("][farmRunning:");
    Serial.print(cityStatus.farmRunning);
    Serial.print("][waterRunning:");
    Serial.print(cityStatus.waterRunning);
    Serial.print("][lightRunning:");
    Serial.print(cityStatus.lightRunning);
    Serial.print("][numLightsOk:");
    Serial.print(cityStatus.numLightsOk);
    Serial.print("][numLightsBroken:");
    Serial.print(cityStatus.numLightsBroken);
    Serial.print("][lightBrokenList:");
    Serial.print(cityStatus.lightRunning);
    Serial.print("][lightsTotalConsum:");
    Serial.print(cityStatus.lightsTotalConsum);
    Serial.print("][lightsTotalCost:");
    Serial.print(cityStatus.lightsTotalCost);
    Serial.print("][farmWatering:");
    Serial.print(cityStatus.farmWatering);
    Serial.print("][farmRoofPosition:");
    Serial.print(cityStatus.farmRoofPosition);
    Serial.print("][farmTempOk:");
    Serial.print(cityStatus.farmTempOk);
    Serial.print("][waterOk:");
    Serial.print(cityStatus.waterOk);
    Serial.print("][waterTankLevel:");
    Serial.print(cityStatus.waterTankLevel);
    Serial.print("][waterPressure:");
    Serial.print(cityStatus.waterPressure);
    Serial.print("][waterPumpPercentage:");
    Serial.print(cityStatus.waterPumpPercentage);
    Serial.print("]");
    Serial.println();
}
