#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdint.h>
#include "time_compare.h"
#define motor 12

//DEFINITIONS FOR SERVO_MOTOR
#define COUNT_LOW 1638
#define COUNT_HIGH 9828
#define TIMER_WIDTH 16

extern float actual_weight;
extern float desire_weight;
extern volatile uint32_t sample_time;
extern volatile uint32_t time_delay;
extern volatile uint32_t count_time_delay;
/*----------------------------*/
/*FUNCTION DECLARATION*/
void state_machine_run(void);

#endif
