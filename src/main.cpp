#include <Arduino.h>
#include "lab1_1/lab1App.h"
#include "lab1_2/lab1_2App.h"
#include "lab2_1/lab2_1App.h"

// Lab selection
#define LAB_1_1 1
#define LAB_1_2 2
#define LAB_2_1 3
#define APP_NAME LAB_2_1

void setup() {
  #if APP_NAME == LAB_1_1
    lab1AppSetup();
  #elif APP_NAME == LAB_1_2
    lab1_2AppSetup();
  #elif APP_NAME == LAB_2_1
    lab2_1AppSetup();
  #endif
}

void loop() {
  #if APP_NAME == LAB_1_1
    lab1AppLoop();
  #elif APP_NAME == LAB_1_2
    lab1_2AppLoop();
  #elif APP_NAME == LAB_2_1
    lab2_1AppLoop();
  #endif
}
