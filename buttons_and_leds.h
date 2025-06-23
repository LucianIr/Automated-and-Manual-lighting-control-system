//start of buttons and leds API

#ifndef BUTTONS_AND_LED_H
#define BUTTONS_AND_LED_H

#include "typedefs.h"

//----------------------------DEFINES
#define ONE_BIT_MASK              0x01
#define ON                        0x01
#define OFF                       0x00


#define BUTTON1                   (0x00)//14
#define BUTTON2                   (0x01)//15
#define BUTTON3                   (0x02)//16
#define BUTTON4                   (0x03)//17

#define BTN1_INDEX                0x00
#define BTN2_INDEX                0x01
#define BTN3_INDEX                0x02
#define BTN4_INDEX                0x03

#define NUMBER_OF_RECCURENCES     0x12 
#define NUMBER_OF_BUTTONS         0x04
//----------------------------END


//----------------------------GLOBAL VARIABLES
typedef struct button
{
  uint16 debounceStatusCounter; 
  uint8 debounceStatusRecurrence; 
  uint8 buttonIsPressed;  
  uint8 button_currentState; 
  uint8 button_oldState; 
  uint8 isDebounceFinished;
  uint8 buttonPin; 
}BUTTONS;

extern BUTTONS buttonsVector[NUMBER_OF_BUTTONS];
//----------------------------END


//----------------------------GLOBAL FUNCTIONS
extern void all_buttons_init();
extern void execute_passive_debounce(BUTTONS button);
extern void Set_PinMode(uint8 pin);
extern void Use_Button(uint8 pin);
extern uint8 Read_Btn(uint8 pin);
//----------------------------END

#endif