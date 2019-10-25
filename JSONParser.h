#ifndef KERAPOLIS_JSONPARSER_H
#define KERAPOLIS_JSONPARSER_H

#include "CityInfo.h"
#include "CityStatus.h"


extern CityInfo cityInfo;
extern CityStatus cityStatus;

void parseInfo(char *data);
void parseEvent(char *data);
std::string encodeStatus();

#endif
