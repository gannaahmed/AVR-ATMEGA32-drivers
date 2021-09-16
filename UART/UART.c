/******************************************************************************
 *
 * Module: UART
 *
 * File Name: UART.c
 *
 * Description: Source file for UART AVR driver
 *
 * Author: Ganna Ahmed
 *
 *******************************************************************************/

/*******************************************************************************
 *                     			  INCLUDES                                     *
 *******************************************************************************/
#include "UART.h"


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*************************************************************************************************
 *  [Function Name]:    UART_init
 *  [Description] :		This Function initializes UART
 *                      1-Set required Baud rate
 *                      2-Set required number of data bits in frame
 *                      3-Set required parity
 *                      4-Set required number of Stop bits
 *  [Args] :            Pointer to Struct UART_ConfigType
 *  [Returns] :			NONE
 ***************************************************************************************************/
void UART_init(const UART_ConfigType * config_Ptr){

	SET_BIT(UCSRA,U2X) ; /* U2X = 1 for double transmission speed */

	/************************** UCSRB Description **************************
	 * RXCIE = 0 Disable USART RX Complete Interrupt Enable
	 * TXCIE = 0 Disable USART Tx Complete Interrupt Enable
	 * UDRIE = 0 Disable USART Data Register Empty Interrupt Enable
	 * RXEN  = 1 Receiver Enable
	 * RXEN  = 1 Transmitter Enable
	 * UCSZ2 = 0 For 8-bit data mode
	 * RXB8 & TXB8 not used for 8-bit data mode
	 ***********************************************************************/
	UCSRB = (1<<RXEN) | (1<<TXEN);
	UCSRB = (UCSRB & 0xFB) | ((config_Ptr->s_dataBits) & 0x04);  /* setting UCSZ2 bit for the required data bit mode */

	/************************** UCSRC Description **************************
	 * URSEL   = 1 The URSEL must be one when writing the UCSRC  ,0 when writing in UBRR
	 * UMSEL   = 0 Asynchronous Operation
	 * UPM1:0  = 00 Disable parity bit
	 * USBS    = 0 One stop bit
	 * UCSZ1:0 = 11 For 8-bit data mode
	 * UCPOL   = 0 Used with the Synchronous operation only
	 ***********************************************************************/
	UCSRC = (1<<URSEL);
	UCSRC = (UCSRC & 0xF9) | (((config_Ptr->s_dataBits) & 0x03)<<1); /* setting UCSZ1:0 bits for the required data bit mode */
	UCSRC = (UCSRC & 0xCF) | ((config_Ptr->s_parity)<<4); /* setting UPM1:0 bits for the required parity type */
	UCSRC = (UCSRC & 0xF7) | ((config_Ptr->s_stopBits)<<3); /* setting USBS bit for the required number of stop bits*/


	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
	CLEAR_BIT(UCSRC,URSEL); /* Clearing URSEL to write in UBRR (because UCSRC & UBRR share address location in memory)  */
	UBRRL = (((F_CPU / ((config_Ptr->s_UART_BaudRate) * 8UL))) - 1);  /* Equation to calculate required baud rate prescale(UBRR) (from data sheet) */
	UBRRH = (((F_CPU / ((config_Ptr->s_UART_BaudRate) * 8UL))) - 1)>>8;

}

/*************************************************************************************************
 *  [Function Name]:    UART_sendByte
 *  [Description] :		This Function transmits 1 byte using pooling
 *  [Args]				uint8 data
 *                         The character that will be sent
 *  [Returns] :			NONE
 ***************************************************************************************************/

void UART_sendByte(const uint8 data){

	/* UDRE flag is set when the Tx buffer (UDR) is empty and ready for
	 * transmitting a new byte so wait until this flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,UDRE)){}
	/* Put the required data in the UDR register and it also "clear the UDRE flag" as
	 * the UDR register is not empty now */
	UDR = data;
	/************************* Another Method *************************
	UDR = data;
	while(BIT_IS_CLEAR(UCSRA,TXC)){} // Wait until the transmission is complete TXC = 1
	SET_BIT(UCSRA,TXC); // Clear the TXC flag
	*******************************************************************/
}



/*************************************************************************************************
 *  [Function Name]:    UART_recieveByte
 *  [Description] :		This Function receives 1 byte using pooling
 *  [Args] :            NONE
 *  [Returns] :	    	uint8
 *                          The received character (1 byte)
 ***************************************************************************************************/

uint8 UART_recieveByte(void){

	/* RXC flag is set when the UART receive data so wait until this
	 * flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,RXC)){}
	/* Read the received data from the Rx buffer (UDR) and the "RXC flag
	   will be cleared" after read this data */
    return UDR;
}


/*************************************************************************************************
 *  [Function Name]:    UART_sendString
 *  [Description] :		This Function transmits a string
 *  [Args] :            const uint8 *Str
 *                          The string that will be sent
 *  [Returns] :	    	NONE
 ***************************************************************************************************/
void UART_sendString(uint8 *Str)
{
	uint8 i = 0;
	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}
	 Str[i]='#';  /* putting '#' as the last byte as a terminal for the string (line'\0' but I can't send it) "#'  */
	 UART_sendByte(Str[i]);
	/************************* Another Method *************************
	while(*Str != '\0')
	{
		UART_sendByte(*Str);
		Str++;
	}
	*******************************************************************/
}

/*************************************************************************************************
 *  [Function Name]:    UART_receiveString
 *  [Description] :		This Function receives a string
 *  [Args] :            uint8 *Str
 *                         The string that the received string will be stored in
 *  [Returns] :	    	NONE
 ***************************************************************************************************/
void UART_receiveString(uint8 *Str)
{
	uint8 i = -1;
	/* Looping on the  bytes until my chosen terminal '#'  */
	do {
		i++;
		Str[i] = UART_recieveByte();
	} while(Str[i] != '#');

	Str[i] = '\0';
}
