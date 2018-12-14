/*
 * DS1307RTC.h - library for DS1307 RTC
 * This library is intended to be uses with Arduino Time.h library functions
 */

#ifndef NTP_UPDATE_h
#define NTP_UPDATE_h

#include <Wire.h>
#include <WiFi.h>
#include "Time.h"
#include "NTPClient.h"
#include "DS1307RTC.h"
#include <WiFiUdp.h>

extern tmElements_t tm;
void NTP_update_init(void);
void updateRtc(void);
String tm2string(tmElements_t *tm);
String read_actual_time(void);
#endif
 
