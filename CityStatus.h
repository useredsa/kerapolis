#ifndef KERAPOLIS_CITYSTATUS_H
#define KERAPOLIS_CITYSTATUS_H

#include <string>
#include <vector>

class CityStatus
{
	public:
		CityStatus();
		std::string frameTime;
		bool farmRunning;
		bool waterRunning;
		bool lightRunning;
		int numLightsOk;
		int numLightsBroken;
		std::vector<char> lightsBrokenList;
		int lightsTotalConsum;
    int lightsTotalCost;
		bool farmWatering;
		int farmRoofPosition;
		bool farmTempOk;
		bool waterOk;
		char waterTankLevel;
		int  waterPressure;
		int waterPumpPercentage;
};

#endif
