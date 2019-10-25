
#ifndef TKERAPolisStatusH
#define TKERAPolisStatusH

#include <string>
#include <vector>

class TKERAPolisStatus
{
	public:
		TKERAPolisStatus();
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
