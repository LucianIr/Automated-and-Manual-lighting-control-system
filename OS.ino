#include "OS.h"

uint8 bulbDutyCycle = 1;
uint8 lightSetForPresence = false; //flag pentru prezență
uint8 application_State;

uint8 OS_tick_1ms = 0; //flag pus pe 1 de ISR o dată la 1ms (semnal pentru OS că trebuie să ruleze task-urile)

//APDS9930 apds = APDS9930();
float ambient_light = 0; // can also be an unsigned long

//-----------Structura unui task
typedef struct task
{
  uint16 recurence;              //La câte milisecunde să ruleze task-ul
  uint16 counter;                //Contor care crește la fiecare ms
  void (*ptrToFunctionInit)();   //Funcție de inițializare pentru task
  void (*ptrToFunctionCyclic)(); //Funcția efectivă care se rulează periodic
}TASKS;
//------------END

TASKS schedulerVector[NUMBER_OF_TASKS]; //vectorul de task-uri

APDS9930 apds;

//Se inițializează toate task-urile 
void OS_init()
{
  schedulerVector[FIRST_TASK_INDEX].ptrToFunctionInit = task_init_5ms;
  schedulerVector[SECOND_TASK_INDEX].ptrToFunctionInit = task_init_10ms;
  schedulerVector[THIRD_TASK_INDEX].ptrToFunctionInit = task_init_100ms;
  schedulerVector[FOURTH_TASK_INDEX].ptrToFunctionInit = task_init_200ms;

  schedulerVector[FIRST_TASK_INDEX].ptrToFunctionInit();
  schedulerVector[SECOND_TASK_INDEX].ptrToFunctionInit();
  schedulerVector[THIRD_TASK_INDEX].ptrToFunctionInit();
  schedulerVector[FOURTH_TASK_INDEX].ptrToFunctionInit();

  Use_Timer1_Interrupt(); 


  all_buttons_init();

  Serial.begin(9600);

  application_State = RUN_TIME_STATE;

  Set_PinMode(LED1);
  //Set_PinMode(SENSOR, INPUT);
  Set_PinMode(LIGHTCONTROL);
  Set_PinMode(RELAY);

  sei();

  Serial.println(F("--------------------------------"));
  Serial.println(F("APDS-9930 - Ambient light sensor"));
  Serial.println(F("--------------------------------"));
  if ( apds.init() ) {
    Serial.println(F("APDS-9930 initialization complete"));
  } else {
    Serial.println(F("Something went wrong during APDS-9930 init!"));
  }
  
  if ( apds.enableLightSensor(false) ) {
    Serial.println(F("Light sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during light sensor init!"));
  }

  // Wait for initialization and calibration to finish
  addDelay(500);

}

// Main task runner
void OS_run()
{
  if(OS_tick_1ms == ON)
  {
    for(uint8 index = 0; index < NUMBER_OF_TASKS; index++)
    {
      schedulerVector[index].counter++;  

      if(schedulerVector[index].counter >= schedulerVector[index].recurence) 
      {
        schedulerVector[index].ptrToFunctionCyclic(); 
        schedulerVector[index].counter = 0; 
      }
    }
    OS_tick_1ms = OFF;
  }
}

// Se configurează Timer1 să seteze o întrerupere la fiecare 1ms
void Use_Timer1_Interrupt()
{
  TCCR1A = 0x00; 
  TCCR1B = 0x0B;               //timer pornit cu prescaler 64 in CTC mode
  TIMSK1 = 0x02;                 //activează întreruperea pentru OCR1AL

  OCR1AH = 0x00;
  OCR1AL = 0xFA;                //valoarea la care face comparația 

  TCNT1 = 0x00;
}

//Funcția de întrerupere care se execută automat la 1ms


ISR(TIMER1_COMPA_vect)
{
  OS_tick_1ms = ON;
}


// Funcție care citește starea senzorului de prezență și pornește/ oprește senzorul APDS
void checkPresenceAndControlAPDS() 
{
  uint8 isPresenceDetected = digitalRead(SENSOR);
  if (isPresenceDetected == ON) 
  {
    digitalWrite(RELAY,ON);
    if(lightSetForPresence == false)
    {
      apds.readAmbientLightLux(ambient_light);
      Serial.print("Lumina ambientala: ");
      Serial.println(ambient_light);
      if(ambient_light <= 5)
      {
        analogWrite(LIGHTCONTROL,1);
      }
      if(ambient_light > 5 && ambient_light <= 20)
      {
        analogWrite(LIGHTCONTROL,50);
      }
      if(ambient_light > 20 && ambient_light <=100 )
      {
        analogWrite(LIGHTCONTROL,150);
      }
      if(ambient_light > 100 && ambient_light <=300 )
      {
        analogWrite(LIGHTCONTROL,200);
      }
      if(ambient_light > 300 && ambient_light <= 1500 )
      {
        digitalWrite(LIGHTCONTROL, LOW);
      }
      lightSetForPresence = true;
    }
  } 
  else 
  {
    digitalWrite(RELAY,LOW);
    lightSetForPresence = false; 
  }
}

void check_SwitchState()
{
  
  if(buttonsVector[BTN1_INDEX].isDebounceFinished == true)
  {
    application_State = MANUAL_STATE;
    if(0 == digitalRead(LED1))
    {
      digitalWrite(LED1, ON);
    }
  }
  
  if(buttonsVector[BTN2_INDEX].isDebounceFinished == true)
  {
    application_State = RUN_TIME_STATE;
    if(1 == digitalRead(LED1))
    {
      digitalWrite(LED1, OFF);
    }
    buttonsVector[BTN2_INDEX].isDebounceFinished = false;
  }
}


void execute_relayToggle()
{
  uint8 staterelay = digitalRead(RELAY);
  
  if (staterelay == OFF)
  {
    digitalWrite(RELAY,ON);
    analogWrite(LIGHTCONTROL,bulbDutyCycle);
  }
  else
  {
    digitalWrite(RELAY,OFF); 
  }
}

void execute_manualState()
 {

  if(buttonsVector[BTN1_INDEX].isDebounceFinished == true)
  {

    execute_relayToggle();

    buttonsVector[BTN1_INDEX].isDebounceFinished = false;

  }
  if(buttonsVector[BTN3_INDEX].isDebounceFinished == true)
  {

    if (bulbDutyCycle >= 201) 
    {
      bulbDutyCycle = 201;
    }   
    else 
    {
      bulbDutyCycle += 25;
    }
    
    analogWrite(LIGHTCONTROL, bulbDutyCycle);

    buttonsVector[BTN3_INDEX].isDebounceFinished = false;
    
  }

  if(buttonsVector[BTN4_INDEX].isDebounceFinished == true)
  {
    if (bulbDutyCycle <= 1) 
    {
      bulbDutyCycle = 1;
    }   
    else 
    {
      bulbDutyCycle -= 25;
    }
  
    analogWrite(LIGHTCONTROL, bulbDutyCycle);

    buttonsVector[BTN4_INDEX].isDebounceFinished = false;
  }

  Serial.println(bulbDutyCycle);
}


// Holds state of application
void state_application_machine()
{
  check_SwitchState();
  
  if(application_State == RUN_TIME_STATE)
  {
    checkPresenceAndControlAPDS();
  }

  if(application_State == MANUAL_STATE)
  {
    execute_manualState();
  }

}

// Function to check state of buttons and execute debounce
void check_BTNS()
{
  execute_passive_debounce(&buttonsVector[BTN1_INDEX]);
  execute_passive_debounce(&buttonsVector[BTN2_INDEX]);
  execute_passive_debounce(&buttonsVector[BTN3_INDEX]);
  execute_passive_debounce(&buttonsVector[BTN4_INDEX]);
}

void addDelay(uint16 delayVal)
{
  delayVal = delayVal - ((delayVal / 100) * 24);
  while(delayVal >= 5)
  {
    Serial.println(delayVal);
    delayVal -= 1;
    __asm__ __volatile__("nop");
  }
}


// 5ms Cyclic task
void task_cyclic_5ms()
{
  check_BTNS();
}

// 10ms Cyclic task
void task_cyclic_10ms()
{
  
}

// 100ms Cyclic task
void task_cyclic_100ms()
{
  state_application_machine();
}

// 200ms Cyclic task
void task_cyclic_200ms()
{
  
}

// task init 5ms
void task_init_5ms()
{
  schedulerVector[FIRST_TASK_INDEX].recurence = 5; 
  schedulerVector[FIRST_TASK_INDEX].counter = 1;   
  schedulerVector[FIRST_TASK_INDEX].ptrToFunctionCyclic = task_cyclic_5ms; 
}

// task init 10ms
void task_init_10ms()
{
  schedulerVector[SECOND_TASK_INDEX].recurence = 10;
  schedulerVector[SECOND_TASK_INDEX].counter = 2;
  schedulerVector[SECOND_TASK_INDEX].ptrToFunctionCyclic = task_cyclic_10ms;
}

// task init 100ms
void task_init_100ms()
{
  schedulerVector[THIRD_TASK_INDEX].recurence = 100;
  schedulerVector[THIRD_TASK_INDEX].counter = 5;
  schedulerVector[THIRD_TASK_INDEX].ptrToFunctionCyclic = task_cyclic_100ms;
}

// task init 200ms
void task_init_200ms()
{
  schedulerVector[FOURTH_TASK_INDEX].recurence = 200;
  schedulerVector[FOURTH_TASK_INDEX].counter = 7;
  schedulerVector[FOURTH_TASK_INDEX].ptrToFunctionCyclic = task_cyclic_200ms;
}

