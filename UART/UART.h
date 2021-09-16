/******************************************************************************
 *
 * Module: UART
 *
 * File Name: UART.h
 *
 * Description: UART header file
 *
 * Author: Ganna Ahmed
 *
 *******************************************************************************/
#ifndef UART_H_
#define UART_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_configurations.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

typedef enum
{
	five,six,seven,eight,nine=7
}UART_NumberOfDataBits;

typedef enum
{
	disabled,reserved,even,odd
}UART_ParityType;

typedef enum
{
	one,two
}UART_NumberOfStopBits;

typedef struct
{
	uint32 s_UART_BaudRate;
	UART_NumberOfDataBits s_dataBits;
	UART_ParityType s_parity;
	UART_NumberOfStopBits s_stopBits;
}UART_ConfigType;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void UART_init(const UART_ConfigType * config_Ptr);
void UART_sendByte(const uint8 data);
uint8 UART_recieveByte(void);
void UART_sendString(uint8 *Str);
void UART_receiveString(uint8 *Str); /* Receive until '#'  */


#endif /* UART_H_ */
