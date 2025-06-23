//start of OS API

#ifndef OS_H
#define OS_H
#include "buttons_and_leds.h"

//----------------------------DEFINES

//Indexes for the four tasks in the scheduler vector
#define FIRST_TASK_INDEX        0x00 
#define SECOND_TASK_INDEX       0x01
#define THIRD_TASK_INDEX        0x02
#define FOURTH_TASK_INDEX       0x03
//
 
#define LED1                       3
#define SENSOR                     7
#define LIGHTCONTROL               6
#define RELAY                      8

#define RUN_TIME_STATE          0x01
#define MANUAL_STATE            0X02

#define NUMBER_OF_TASKS         0x04 //numărul de task-uri
//----------------------------END


#define ON                         1
#define OFF                        0


//----------------------------GLOBAL FUNCTIONS
extern void OS_init();
extern void OS_run();
extern void Use_Timer_Interrupt();
//----------------------------END

#endif