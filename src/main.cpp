#include <Arduino.h>
#include "lab1_1/lab1App.h"
#include "lab1_2/lab1_2App.h"
#include "lab2_1/lab2_1App.h"
#include "lab2_2/lab2_2App.h"
#include "lab3_1/lab3_1App.h"
#include "lab3_2/lab3_2App.h"
#include "lab4_1/lab4_1App.h"

// Lab selection
#define LAB_1_1 1
#define LAB_1_2 2
#define LAB_2_1 3
#define LAB_2_2 4
#define LAB_3_1 5
#define LAB_3_2 6
#define LAB_4_1 7
#define APP_NAME LAB_4_1

void setup() {
  #if APP_NAME == LAB_1_1
    lab1AppSetup();
  #elif APP_NAME == LAB_1_2
    lab1_2AppSetup();
  #elif APP_NAME == LAB_2_1
    lab2_1AppSetup();
  #elif APP_NAME == LAB_2_2
    lab2_2AppSetup();
  #elif APP_NAME == LAB_3_1
    lab3_1AppSetup();
  #elif APP_NAME == LAB_3_2
    lab3_2AppSetup();
  #elif APP_NAME == LAB_4_1
    lab4_1AppSetup();
  #endif
}

void loop() {
  #if APP_NAME == LAB_1_1
    lab1AppLoop();
  #elif APP_NAME == LAB_1_2
    lab1_2AppLoop();
  #elif APP_NAME == LAB_2_1
    lab2_1AppLoop();
  #elif APP_NAME == LAB_2_2
    lab2_2AppLoop();
  #elif APP_NAME == LAB_3_1
    lab3_1AppLoop();
  #elif APP_NAME == LAB_3_2
    lab3_2AppLoop();
  #elif APP_NAME == LAB_4_1
    lab4_1AppLoop();
  #endif
}
