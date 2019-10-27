#ifndef KERAPOLIS_CITY_H
#define KERAPOLIS_CITY_H

#include <string>
#include <vector>

struct Time {
  static char printBuff[9];
  int h = 0, m = 0, s = 0;

  void fromString(const char* str);
  char* toString();
  bool between(Time& L, Time& R);
};

struct CityInfo {
	Time time;
	double lightPrice = 0.0;
	double temperature = 0.0;
	int humidity = 0;

  Time& localTime();
  int timeZone();
};


struct CityStatus {
  Time frameTime;
  bool farmRunning = false;
  bool waterRunning = false;
  bool lightRunning = false;
  int numLightsOk = 0;
  int numLightsBroken = 0;
  std::vector<char> lightsBrokenList;
  int lightsTotalConsum = 0;
  int lightsTotalCost = 0;
  bool farmWatering = false;
  int farmRoofPosition = 0;
  bool farmTempOk = false;
  bool waterOk = false;
  int waterTankLevel = 0;
  int  waterPressure = 0;
  int waterPumpPercentage = 0;
};

class CityEvent {
  public:
    enum class Status{UNSET, NOT_STARTED, STARTED};
    Status status = Status::UNSET;
    Time receivedTime;
    int zone;
    Time startTime;
    Time endTime;
    std::string description;
};

extern CityInfo cityInfo;
extern CityStatus cityStatus;
const int maxNumEvents = 10;
//int numEvents = 0;
extern CityEvent cityEvents[maxNumEvents];

#endif
