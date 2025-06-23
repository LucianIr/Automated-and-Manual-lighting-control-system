#include <avr/io.h>
#include <avr/interrupt.h>
#include "OS.h"
#include <Wire.h>
#include <APDS9930.h>


void setup() {
  OS_init();
}

void loop() {

  OS_run();
}



