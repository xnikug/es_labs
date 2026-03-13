/**
 * @file lab3_1App.h
 * @brief Lab 3.1 application entry points.
 *
 * Defines setup/loop functions used by main.cpp to bootstrap
 * the Lab 3.1 FreeRTOS application.
 */

#ifndef LAB3_1_APP_H
#define LAB3_1_APP_H

/**
 * @brief Initializes services, shared state, drivers and tasks for Lab 3.1.
 */
void lab3_1AppSetup();

/**
 * @brief Arduino loop hook for Lab 3.1.
 *
 * Intentionally empty because FreeRTOS tasks run the application.
 */
void lab3_1AppLoop();

#endif // LAB3_1_APP_H
