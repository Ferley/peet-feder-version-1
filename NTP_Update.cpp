
#include "NTP_Update.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

tmElements_t tm;

void NTP_update_init(void){
  timeClient.begin();
  timeClient.setTimeOffset(-18000);
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
  Serial.print("Update RTC from NTP server");
  updateRtc();

}
/*------------------------------------------*/
/*----------function for RTC update---------*/
/*------------------------------------------*/
void updateRtc(void){
  //unsigned long rawTime = timeClient.getEpochTime();  // in days
  time_t rawtime = timeClient.getEpochTime();
  struct tm * ti;
  ti = localtime(&rawtime);
  /*copi to tm from ti*/
  tm.Second=ti->tm_sec; 
  tm.Minute=ti->tm_min; 
  tm.Hour=ti->tm_hour; 
  tm.Wday=ti->tm_wday;   // day of week, sunday is day 1
  tm.Day=ti->tm_mday;
  tm.Month=ti->tm_mon+1; 
  tm.Year=ti->tm_year-70;  
  RTC.write(tm);
  Serial.print("dateTime update to: ");
  Serial.println(read_actual_time());
  }

  String tm2string(tmElements_t *tm){
   uint16_t year = tm->Year + 1970;
   String yearStr = String(year);

   uint8_t month = tm->Month;
   String monthStr = month < 10 ? "0" + String(month) : String(month);

   uint8_t day = tm->Day;
   String dayStr = day < 10 ? "0" + String(day) : String(day);

   uint8_t hours = tm->Hour;
   String hoursStr = hours < 10 ? "0" + String(hours) : String(hours);

   uint8_t minutes = tm->Minute;
   String minuteStr = minutes < 10 ? "0" + String(minutes) : String(minutes);

   uint8_t seconds = tm->Second;
   String secondStr = seconds < 10 ? "0" + String(seconds) : String(seconds);

   return yearStr + "-" + monthStr + "-" + dayStr + " " +
          hoursStr + ":" + minuteStr + ":" + secondStr;
  }

  String read_actual_time(void){
    RTC.read(tm);
    return tm2string(&tm);
    }
