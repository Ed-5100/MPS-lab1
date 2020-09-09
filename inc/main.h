//------------------------------------------------------------------------------------
// Hello.h
//------------------------------------------------------------------------------------
//
// Author:
//  Karl Nasrallah
//
// Function prototype definitions and related for hello.c
//

#ifndef __HELLO_H
#define __HELLO_H

//------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------
#include "init.h" // Always need init.h, otherwise nothing will work.
#include <vector>
#include <string>
using namespace std;
//------------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------------

void task12_init();
void gpio_hal_init();
void task3_hal_update_led();
void task4_init(vector<string>& maze, pair<int,int>& posit);
void task4_update(vector<string>& maze, pair<int,int> &posit,int reset[2]);
void HAL_GPIO_EXTI_Callback (uint16_t GPIO_Pin);
int task12_update_screen();
void task3_register_pin_set();
void task3_register_update_led();

//------------------------------------------------------------------------------------
// Global Variables
//------------------------------------------------------------------------------------
UART_HandleTypeDef Second_UART;

#endif /* __HELLO_H */
