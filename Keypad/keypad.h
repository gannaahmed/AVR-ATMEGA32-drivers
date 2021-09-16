 /******************************************************************************
 *
 * Module:KEYPAD
 *
 * File Name: std_types.h
 *
 * Description: types for AVR
 *
 * Author: Ganna Ahmed
 *
 *******************************************************************************/

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include"std_types.h"
#include"micro_configurations.h"
#include"common_macros.h"

/* Keypad configurations for number of rows and columns */
#define N_col 4
#define N_row 4

/* Keypad Port Configurations */
#define KEYPAD_PORT_OUT PORTC
#define KEYPAD_PORT_IN  PINC
#define KEYPAD_PORT_DIR DDRC

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

uint8 keypad_getPressedKey(void);

#endif /* KEYPAD_H_ */
