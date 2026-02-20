#include <Arduino.h>
#include "lab1_1/lab1App.h"
#include "lab1_2/lab1_2App.h"

// Lab selection
#define LAB_1_1 1
#define LAB_1_2 2
#define APP_NAME LAB_1_2

void setup() {
  #if APP_NAME == LAB_1_1
    lab1AppSetup();
  #elif APP_NAME == LAB_1_2
    lab1_2AppSetup();
  #endif
}

void loop() {
  #if APP_NAME == LAB_1_1
    lab1AppLoop();
  #elif APP_NAME == LAB_1_2
    lab1_2AppLoop();
  #endif
}
