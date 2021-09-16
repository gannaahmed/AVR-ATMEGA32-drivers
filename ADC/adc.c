 /******************************************************************************
 *
 * Module: ADC
 *
 * File Name: adc.c
 *
 * Description: Source file for the ADC driver
 *
 * Author: Ganna Ahmed
 *
 *******************************************************************************/

/*******************************************************************************
 *                     			  INCLUDES                                     *
 *******************************************************************************/
#include "ADC.h"

/*******************************************************************************
 *                          Global Variables                                   *
 *******************************************************************************/
#ifdef ADC_INTERRUPT
volatile uint16 g_adcResult = 0;
#endif

/*******************************************************************************
 *                          ISR's Definitions                                  *
 *******************************************************************************/
/* Interrupt when ADC conversion is complete */
#ifdef ADC_INTERRUPT
ISR(ADC_vect)
{
	/* Read ADC Data after conversion complete */
	g_adcResult = ADC;
}
#endif

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

 /******************************************************************************
 *
 * [Function name]: ADC_init
 *
 * [Description]: function  that initialize the ADC
 *
 * [Args]: NONE
 *
 * [returns]: NONE
 *
 *******************************************************************************/
void ADC_init(void){

	/* ************************** ADMUX Description **************************
	 * REFS1:0 = 00 to choose to connect external reference voltage by input this voltage through AREF pin
	 * ADLAR   = 0 right adjusted
	 * MUX4:0  = 00000 to choose channel 0 as initialization
	 */
	ADMUX = 0;

	/* ************************** ADCSRA Description **************************
	 * ADEN    = 1 Enable ADC
	 * ADIE    = 1 Enable ADC Interrupt
	 * ADPS2:0 = 011 to choose ADC_Clock=F_CPU/8=1Mhz/8=125Khz --> ADC must operate in range 50-200Khz
	 */
	ADCSRA = (1<<ADEN) | (1<<ADIE) | (1<<ADPS1) | (1<<ADPS0);

	/* In case working with Interrupt , ADIE = 1 Enable ADC Interrupt */
#ifdef ADC_INTERRUPT
	SET_BIT(UCSRB,ADIE);
#endif
}


#ifdef ADC_INTERRUPT
/******************************************************************************
*
* [Function name]: ADC_readChannel_INT
*
* [Description]: the function reads certain channel and start conversion using
*                ADC and gives interrupt when conversion done
*
* [Args]: channel_num:
*                    channel number (from 0 to 7)
*
* [returns]: NONE
*
* [Remarks]: In main extern g_adcResult to get ADC converted value
*
*******************************************************************************/

void ADC_readChannel_INT(uint8 channel_num)
{
	/* clear first 5 bits in the ADMUX (channel number MUX4:0 bits) before set the required channel
	 *  choose the correct channel by setting the channel number in MUX4:0 bits     */
	ADMUX= (ADMUX & 0xE0) | (channel_num & 0x07);

	SET_BIT(ADCSRA,ADSC); /* start conversion write '1' to ADSC */
}

#else
/******************************************************************************
*
* [Function name]: ADC_readChannel
*
* [Description]: the function reads certain channel and returns converted value
*                using pooling
*
* [Args]: channel_num:
*                    channel number (from 0 to 7)
*
* [returns]: final converted value
*
*******************************************************************************/

uint16 ADC_readChannel(uint8 channel_num){
	/* clear first 5 bits in the ADMUX (channel number MUX4:0 bits) before set the required channel
	 *  choose the correct channel by setting the channel number in MUX4:0 bits     */
	ADMUX= (ADMUX & 0xE0) | (channel_num & 0x07);
	SET_BIT(ADCSRA,ADSC);

	while (BIT_IS_CLEAR(ADCSRA,ADIF)){

	}
	SET_BIT(ADCSRA,ADIF);  /* start conversion write '1' to ADSC */

	return ADC;
}

#endif


