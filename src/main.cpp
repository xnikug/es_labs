#include <Arduino.h>
#include "lab1_1/lab1App.h"

// Lab selection
#define LAB_1 1
#define APP_NAME LAB_1

void setup() {
  #if APP_NAME == LAB_1
    lab1AppSetup();
  #endif
}

void loop() {
  #if APP_NAME == LAB_1
    lab1AppLoop();
  #endif
}

