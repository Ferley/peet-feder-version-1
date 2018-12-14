#include "time_compare.h"


uint8_t time_compare(void){
  uint8_t i=0;
  bool compare_feed_flag=false; 
  if (RTC.read(tm)) {
    Serial.println(read_actual_time());
    do{
     
      if ((tm.Hour==23)&&(tm.Minute==59)&&(tm.Second>=50)){
        updateRtc();
        for (i=0;  i <= 7; i++) {
          data_struct.feed_time[i].was_fed_flag=false; //clear flag of was_fed_flag
        }
          
      }
      else if ( (tm.Hour==data_struct.feed_time[i].hour)&& \
                (tm.Minute==data_struct.feed_time[i].min)){
                compare_feed_flag=true;
      };
      i++;
      }while(compare_feed_flag==false && i<data_struct.shots);
  }
  if(compare_feed_flag!=true){
    i=255;
  } else{
    i=i-1;
  }
  return i;
}
