#include "state_machine.h"
#include "NTP_Update.h"
#include "connection.h"
#include "load_cell.h"
#include "esp32-hal-ledc.h"


enum state{
  check_event,
  feed,
  check_weight,
  open_cover,
  close_cover,
  check_mqtt_topic,
  adjust_datetime
}actual_state;

const float percent_error=0.05;
float actual_weight=0.0;
float desire_weight;


volatile uint32_t sample_time= 330;
volatile uint32_t time_delay=30;


uint8_t hour_index=255; //  255 means that is not feed hours 

bool was_fed=false;
bool time_to_feed=false;
bool restart_date_time_flag=false;
int i=0;


void state_machine_run(void){
  switch(actual_state){
    case check_event:
    Serial.print(" ENTRO check_event ");
      if( count_time_delay>=sample_time){
        count_time_delay=0;
        hour_index=time_compare();
        if (hour_index!=255){
          if(data_struct.feed_time[hour_index].was_fed_flag==false){
            actual_state=feed; 
            desire_weight= ((float)(data_struct.quantity))*(1.0-percent_error);
            time_delay=1;
          }
        }else if (topic_recive_flag==true){ // Acelerate the topic analizer 
          time_delay=1;
          topic_recive_flag=0;
          actual_state=check_mqtt_topic; 
        }else if(restart_date_time_flag==false){
          actual_state=adjust_datetime; 
          restart_date_time_flag=true;
        }else{
          time_delay=1000;
         actual_state=check_mqtt_topic;
        }
      }
    break;
    
    case feed:
      //implement control here
      digitalWrite(motor,HIGH);
      actual_weight=HX711_read();
      Serial.print("pesoActual: ");
      Serial.println(actual_weight);
      Serial.print("Deseado: ");
      Serial.println(desire_weight);
      if(actual_weight>= desire_weight){
        Serial.print("detener motor...");
        digitalWrite(motor,LOW);
        time_delay=500; // wait stable time
        actual_state=check_weight; 
      } 
      
    break;
    
    case check_weight:
      actual_weight=HX711_read();
      Serial.print("el peso de checkeo es:");
      Serial.println(actual_weight);
      if(actual_weight>= desire_weight){
        actual_state=open_cover; 
      } else{
        Serial.print("seguir sirviendo...");
        actual_state=feed; 
      }
      time_delay=200;
    break;
    
    case open_cover:
      ledcWrite(1,2700);
      time_delay=2000;
      actual_state=close_cover; 
    break;

    case close_cover:
      ledcWrite(1,7400);
      time_delay=300;
      actual_state=check_event; 
      data_struct.feed_time[hour_index].was_fed_flag=true;
    break;

    case check_mqtt_topic:
      check_mqtt();//This function should be called regularly 
      actual_state=check_event; 
    break;

    case adjust_datetime:
      Serial.print("ACTUALIZANDO NTP");
      NTP_update_init();
      Serial.print("date time update from NTP server");
      actual_state=check_event; 
      time_delay=300;
    break;  
  }
}