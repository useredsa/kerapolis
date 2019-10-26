//---------------------------------------------------------------------------

#pragma hdrstop

#include "City.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

CityInfo cityInfo;
CityStatus cityStatus;
//int numEvents = 0;
CityEvent cityEvents[maxNumEvents];

char Time::printBuff[9] = "00:00:00";

void Time::fromString(const char* str) {
  h = ((int)(str[0]-'0'))*10+(int)(str[1]-'0');
  m = ((int)(str[3]-'0'))*10+(int)(str[4]-'0');
  s = ((int)(str[6]-'0'))*10+(int)(str[7]-'0');
}

char* Time::toString() {
  printBuff[0] = '0' + (char) ((h%100)/10);
  printBuff[1] = '0' + (char) h%10;
  printBuff[3] = '0' + (char) ((m%100)/10);
  printBuff[4] = '0' + (char) m%10;
  printBuff[6] = '0' + (char) ((s%100)/10);
  printBuff[7] = '0' + (char) s%10;
  return printBuff;
}

bool Time::between(Time& L, Time& R) {
  return (L.h < h || (L.h == h && L.m < m) || (L.h == h && L.m == m && L.s <= s)) && (R.h > h || (R.h == h && R.m > m) || (R.h == h && R.m == m && R.s >= s));
}

Time& CityInfo::localTime() {
  //TODO update time
  return time;
}

int CityInfo::timeZone() {
  int r = 2*time.h+(time.m>=30);
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
