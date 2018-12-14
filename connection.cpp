#include "connection.h"
#include "NTP_Update.h"

#define MSG_TESTING //comment this define for disable serial data info

WiFiClient espClient;
PubSubClient client(espClient);

data_str data_struct;
String time_string;
const char *mycharp;
 topics_struct topics_labels={
   MAC"/#",
   MAC"/QUANTITY",
   MAC"/SHOTS",
   MAC"/HOURS_",
   MAC"/RECONNECT"
  } ;
char topic_shots_buffer[16]; 
bool topic_recive_flag=false;

void callback_topic(char* topic, byte* payload, unsigned int length) {
  topic_recive_flag=true;
  payload[length]=0;
  /*message only for testing*/
  #if defined(MSG_TESTING)
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    Serial.println((char *)payload);
  #endif
  /**/
  switch(topic[9]){
    case 'Q':
      data_struct.quantity=atoi((char *)payload);
      /*message only for testing*/
      #if definedMSG_TESTING
        Serial.print("Quantity topic was sended:");
        Serial.println(data_struct.quantity);
      #endif
      /**/
    break;
    case 'S':
      data_struct.shots=atoi((char *)payload);
      /*message only for testing*/
      #if defined MSG_TESTING
        Serial.print("Shot topic was sended:");
        Serial.println(data_struct.shots);
      #endif
      /**/
    break;
    case 'H':
      Serial.println("hours topic was sended:");
      uint8_t hour_number=topic[14]-0x31;  
      data_struct.feed_time[hour_number].hour=(payload[0]-0x30)*10+(payload[1]-0x30);
      data_struct.feed_time[hour_number].min=(payload[3]-0x30)*10+(payload[4]-0x30);
      data_struct.feed_time[hour_number].second=0;//(payload[6]-0x30)*10+(payload[7]-0x30);
      
      /*message only for testing*/
      #if defined MSG_TESTING
        uint8_t hora=data_struct.feed_time[hour_number].hour;
        uint8_t minuto=data_struct.feed_time[hour_number].min;
        uint8_t segundo=data_struct.feed_time[hour_number].second;
        Serial.print("Numero de hora: ");
        Serial.println(hour_number);
        Serial.print("Hora: ");
        Serial.println(hora);
        Serial.print("Minuto: ");
        Serial.println(minuto);
        Serial.print("Segundos: ");
        Serial.println(segundo);
      #endif
      /*----------------*/
    break;
    }
      
 }
void connection_mqtt_setup(void){
  WiFi.begin(ssid, password);
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback_topic);
}

/*-------------------------------------------*/
/* This should be called regularly to allow  *
 * the client to process incoming messages   *
 * and maintain its connection to the server *
/*-------------------------------------------*/
bool check_mqtt(void){
    bool connect_flag=false;

    connect_flag=client.loop();
    /*if the device is disconnect, this will try to connect*/
    if(!connect_flag) {
       Serial.print("Connecting to MQTT...from wifi:");
       Serial.println(ssid);
       //if (client.connect("ESP32Client", mqttUser, mqttPassword )) {
       if (client.connect("ESP32Client" )) {
            Serial.print("CONNECTED to MQTT cliente: ");  
            Serial.println(MQTT_SERVER);
            Serial.print("suscribiendoce a todos los topicos: ");  
            Serial.print("CONNECTED to MQTT cliente: ");  
            Serial.println(MQTT_SERVER);
            client.subscribe(topics_labels.all);
            /* send reconnect event to server, please change time for RTC date and time*/
            time_string=read_actual_time();
            mycharp = time_string.c_str();
            client.publish(topics_labels.reconnect,mycharp); 
          } else {
            Serial.print("FAILED!, with state ");
            Serial.println(client.state());
          }
      }
      return connect_flag;
  }
