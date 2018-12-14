#ifndef _CONNECTION_H_
#define _CONNECTION_H_
/*----------------------------*/
#include <WiFi.h>
#include "PubSubClient.h"
#include <stdio.h>
#include <string.h>

/*----------------------------*/
/*MQTT MACRO DEFINITIONS*/
#define MAX_NUMBER_SHOTS 8
#define MAC "12345678"
/*----------------------------*/
/*WIFI CREDENTIALS*/
/*hola*/
#define ferley_router
#if defined(ferley_router)
  #define ssid     "VALLEJO_CA"
  #define password "70757061" 
#else
  #define ssid     "UCO-Inalambrico"
  #define password ""
  //#define ssid     "ECHEVERRI"
 // #define password "Lucas2325" 
#endif
/*----------------------------*/
/*MQTT SERVER AND CREDENTIALS*/
//#define MQTT_SERVER   "35.207.54.102"

#define MQTT_PORT     1883
#define mqttPassword  "Plamontina01"
#define mqttUser      "wedoiot"

const char  MQTT_SERVER[]= "35.207.54.102";

/*STRUCT DEFINITIONS*/
typedef struct{
   char hour;
   char min;
   char second;
   bool was_fed_flag;
  }time_str ;
  
typedef struct{
   time_str feed_time[MAX_NUMBER_SHOTS];
   uint8_t shots;
   uint16_t quantity;
  }data_str ;
  
typedef struct{
   char *all;
   char *quantity;
   char *shots;
   char *hours;
   char *reconnect;
  }topics_struct ;
/*----------------------------*/
/*FUNCTION DECLARATION*/
void connection_mqtt_setup(void);
void callback(char* topic, byte* payload, unsigned int length);
bool check_mqtt(void);

extern data_str data_struct;
extern bool topic_recive_flag;
#endif
