#ifndef KERAPOLIS_JSONPARSER_H
#define KERAPOLIS_JSONPARSER_H

#include "CityInfo.h"
#include "CityStatus.h"

class JSONParser
{
	private:
    void printCityInfo();
    void printCityStatus();

	public:
		JSONParser();
		CityInfo cityInfo;
		CityStatus cityStatus;
		void parseInfo(char *data);
		std::string encodeStatus();

};

#endif
