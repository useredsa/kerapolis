#ifndef TCityJSONParserH
#define TCityJSONParserH

#include <ArduinoJson.h>
#include "TKERAPolisInfo.h"
#include "TKERAPolisStatus.h"

class TCityJSONParser
{
	public:
		TCityJSONParser();
		TKERAPolisInfo cityInfo;
		TKERAPolisStatus cityStatus;
		void getCityInfoFromNetwork(char *data);
		std::string setCityStatusToNetwork();

  private:
    void printCityInfo();
    void printCityStatus();
};

#endif
