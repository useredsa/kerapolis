//---------------------------------------------------------------------------

#pragma hdrstop

#include "TKERAPolisStatus.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TKERAPolisStatus::TKERAPolisStatus()
	:
		frameTime("0:00:00"),
		farmRunning(false),
		waterRunning(false),
		lightRunning(false),
		numLightsOk(0),
		numLightsBroken(0),
		lightsBrokenList{},
		lightsTotalConsum(0),
    	lightsTotalCost(0),
		farmWatering(false),
		farmRoofPosition(0),
		farmTempOk(false),
		waterOk(false),
		waterTankLevel(0),
		waterPressure(0),
		waterPumpPercentage(0)
{

}
