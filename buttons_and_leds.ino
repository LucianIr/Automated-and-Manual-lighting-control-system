#include "buttons_and_leds.h"

#define PORTB_OFFSET 0x08

BUTTONS buttonsVector[NUMBER_OF_BUTTONS];

// Functions
void execute_passive_debounce(BUTTONS *button)
{
  if( (Read_Btn(button->buttonPin) == ON) && (button->isDebounceFinished == false))
  {
    if(button->debounceStatusCounter >= button->debounceStatusRecurrence)
    {
      button->debounceStatusCounter = 0;
      button->isDebounceFinished = true;
    }
    button->button_oldState = button->button_currentState;
    button->button_currentState = Read_Btn(button->buttonPin);

    if(button->button_currentState == button->button_oldState)
    {
      button->debounceStatusCounter++;
    }
    else
    {
      button->debounceStatusCounter = 0;
    }
  } 
}

void BTN1_init()
{
  buttonsVector[BTN1_INDEX].debounceStatusCounter = 0;
  buttonsVector[BTN1_INDEX].debounceStatusRecurrence = NUMBER_OF_RECCURENCES;
  buttonsVector[BTN1_INDEX].buttonIsPressed = 0;
  buttonsVector[BTN1_INDEX].button_currentState = ON;
  buttonsVector[BTN1_INDEX].button_oldState = OFF;
  buttonsVector[BTN1_INDEX].isDebounceFinished = 0;
  buttonsVector[BTN1_INDEX].buttonPin = BUTTON1;

  Use_Button(BUTTON1);
}

void BTN2_init()
{
  buttonsVector[BTN2_INDEX].debounceStatusCounter = 0;
  buttonsVector[BTN2_INDEX].debounceStatusRecurrence = NUMBER_OF_RECCURENCES;
  buttonsVector[BTN2_INDEX].buttonIsPressed = 0;
  buttonsVector[BTN2_INDEX].button_currentState = ON;
  buttonsVector[BTN2_INDEX].button_oldState = OFF;
  buttonsVector[BTN2_INDEX].isDebounceFinished = 0;
  buttonsVector[BTN2_INDEX].buttonPin = BUTTON2;

  Use_Button(BUTTON2);
}

void BTN3_init()
{
  buttonsVector[BTN3_INDEX].debounceStatusCounter = 0;
  buttonsVector[BTN3_INDEX].debounceStatusRecurrence = NUMBER_OF_RECCURENCES;
  buttonsVector[BTN3_INDEX].buttonIsPressed = 0;
  buttonsVector[BTN3_INDEX].button_currentState = ON;
  buttonsVector[BTN3_INDEX].button_oldState = OFF;
  buttonsVector[BTN3_INDEX].isDebounceFinished = 0;
  buttonsVector[BTN3_INDEX].buttonPin = BUTTON3;

  Use_Button(BUTTON3);
}

void BTN4_init()
{
  buttonsVector[BTN4_INDEX].debounceStatusCounter = 0;
  buttonsVector[BTN4_INDEX].debounceStatusRecurrence = NUMBER_OF_RECCURENCES;
  buttonsVector[BTN4_INDEX].buttonIsPressed = 0;
  buttonsVector[BTN4_INDEX].button_currentState = ON;
  buttonsVector[BTN4_INDEX].button_oldState = OFF;
  buttonsVector[BTN4_INDEX].isDebounceFinished = 0;
  buttonsVector[BTN4_INDEX].buttonPin = BUTTON4;

  Use_Button(BUTTON4);

}

void all_buttons_init()
{
  BTN1_init();
  BTN2_init();
  BTN3_init();
  BTN4_init();
}


void Set_PinMode(uint8 pin)
{
  if( pin > 7)
  {
    DDRB |= (ONE_BIT_MASK << (pin - PORTB_OFFSET));
  }
  else
  {
    DDRD |= (ONE_BIT_MASK << pin);
  }
}

void Use_Button(uint8 pin)
{
  DDRC &= ~(1 << pin);         
  PORTC |= (1 << pin);         
}

uint8 Read_Btn(uint8 pin)
{
  return ((PINC >> pin) & ONE_BIT_MASK) ? 0 : 1; 
}
