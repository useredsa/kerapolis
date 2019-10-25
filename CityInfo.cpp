//---------------------------------------------------------------------------

#pragma hdrstop

#include "CityInfo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

CityInfo::CityInfo()
  : time(""), lightPrice(0.0), temperature(0.0), humidity(0)
{}

int CityInfo::timeZone() {
  int h = ((int)(time[0]-'0'))*10+(int)(time[1]-'0');
  int m = ((int)(time[3]-'0'))*10+(int)(time[4]-'0');
  int r = 2*h+(m>=30);
#ifdef TELL
  if (r < 0 || r >= 48) {
    Serial.print("Wrong timezone: ");
    Serial.print(r);
    Serial.print(" from ");
    Serial.println(time);
    r = 0;
  }
#endif
  return r;
}
