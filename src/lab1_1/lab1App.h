/**
 * @file lab1App.h
 * @brief Application interface for Lab 1.1 - Serial LED Control.
 * 
 * This module defines the setup and loop entry points for the application
 * logic that interprets textual commands to control an LED.
 */

#ifndef LAB1APP_H
#define LAB1APP_H

/**
 * @brief Initializes the application layer.
 * 
 * Sets up the serial service (STDIO), initializes hardware drivers (LED),
 * and displays the welcome message and available commands to the user.
 */
void lab1AppSetup();

/**
 * @brief Main application execution loop.
 * 
 * This function handles the serial input stream, processes characters into
 * a command buffer, and executes commands when a newline is received.
 * It should be called cyclically from the main system loop.
 */
void lab1AppLoop();

#endif