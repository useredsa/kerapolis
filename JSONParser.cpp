//---------------------------------------------------------------------------

#pragma hdrstop

#include "JSONParser.h"
#include <ArduinoJson.h>
#include <string>
//---------------------------------------------------------------------------
#pragma package(smart_init)

JSONParser::JSONParser()
{

}

void JSONParser::getCityInfoFromNetwork(char *inputData)
{
  ArduinoJson::StaticJsonDocument<250> doc;
  ArduinoJson::DeserializationError error = ArduinoJson::deserializeJson(doc, (char *)inputData);
	if (error == ArduinoJson::DeserializationError::Ok)
	{
		cityInfo.time = doc["KERAPolisInfo"]["time"];
		cityInfo.lightPrice = doc["KERAPolisInfo"]["lightPrice"];
		cityInfo.humidity = doc["KERAPolisInfo"]["humidity"];
		cityInfo.temperature = doc["KERAPolisInfo"]["temperature"];
    printCityInfo();
	} else {
    Serial.println("problem parsing json");
	}
}


std::string JSONParser::setCityStatusToNetwork()
{
  
	char statusJSON[800];
	ArduinoJson::StaticJsonDocument<800> doc;

	doc["KERAPolisStatus"]["time"] = cityStatus.frameTime.c_str();
	doc["KERAPolisStatus"]["farmRunning"] = cityStatus.farmRunning;
	doc["KERAPolisStatus"]["waterRunning"] = cityStatus.waterRunning;
	doc["KERAPolisStatus"]["lightRunning"] = cityStatus.lightRunning;
  doc["KERAPolisStatus"]["numLightsOk"] = cityStatus.numLightsOk;
  doc["KERAPolisStatus"]["numLightsBroken"] = cityStatus.numLightsBroken;
  doc["KERAPolisStatus"]["lightsBrokenList"] = cityStatus.lightRunning;
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
  printCityStatus();
	return std::string(statusJSON);
}
    
void JSONParser::printCityInfo()
{
    Serial.print("KERAPolis Info received -->");
    Serial.print("[time:");
    Serial.print(cityInfo.time);
    Serial.print("][humidity:");
    Serial.print(cityInfo.humidity);    
    Serial.print("][temperature:");
    Serial.print(cityInfo.temperature);
    Serial.print("]");
    Serial.println();        
}

void JSONParser::printCityStatus()
{
    Serial.print("KERAPolis STATUS SENT --> ");
    Serial.print("[time:");
    Serial.print(cityStatus.frameTime.c_str());
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