#ifndef KERAPOLIS_CITYINFO_H
#define KERAPOLIS_CITYINFO_H

class CityInfo
{
	public:
        CityInfo();
    		const char *time;
    		double lightPrice;
    		double temperature;
    		int humidity;
        int timeZone();
};

#endif
