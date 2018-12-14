#include "connection.h"
#include "NTP_Update.h"
#include "time_compare.h"
#include "soc/rtc.h"
#include "load_cell.h"
#include "state_machine.h"
#include "esp_system.h"

const int wdtTimeout = 90000;  //time in ms to trigger the watchdog
hw_timer_t *timer = NULL;
volatile uint32_t count_time_delay=0;


//-------------------------------
// Reset watch dog function
//-------------------------------
void IRAM_ATTR resetModule() {
  ets_printf("reboot\n");
  esp_restart();
}

/*------------------------------------------*/
/*-Initialization modules and pheripherical-*/
/*------------------------------------------*/
void setup() {
/*-UART init-*/
  Serial.begin(115200);

//watch dog setup
  Serial.print("watch dog init");
  timer = timerBegin(0, 80, true);                  //timer 0, div 80
  timerAttachInterrupt(timer, &resetModule, true);  //attach callback
  timerAlarmWrite(timer, wdtTimeout * 1000, false); //set time in us
  timerAlarmEnable(timer);                          //enable interrupt

/*-MQTT setup-*/ 
Serial.print("MQTT setup");
  connection_mqtt_setup();

/*-MOTOR setup-*/  
 Serial.print("MOTOR setup");
  pinMode(motor,OUTPUT);
  digitalWrite(motor,LOW);

/*-Servo setup-*/  
  Serial.print("Servo setup");
  ledcSetup(1, 50, TIMER_WIDTH); // channel 1, 50 Hz, 16-bit width
  ledcAttachPin(2, 1);   // GPIO 22 assigned to channel 1
  ledcWrite(1,7400);    //close of servo_motor

/*-HX711 setup-*/ 
  Serial.print("FRECUENCIA CELDA DE CARGA");
  rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M);
  HX711_calibration();

}
/*------------------------------------------*/
/*--------------Main loop-------------------*/
/*------------------------------------------*/
void loop() {
  state_machine_run();
  count_time_delay+=time_delay;
  delay(time_delay);
  timerWrite(timer, 0); //reset timer (feed watchdog)
}
