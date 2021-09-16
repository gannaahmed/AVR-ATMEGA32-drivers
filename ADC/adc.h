 /******************************************************************************
 *
 * Module: ADC
 *
 * File Name: adc.c
 *
 * Description: header file for the ADC
 *
 * Author: Ganna Ahmed
 *
 *******************************************************************************/

#ifndef ADC_H_
#define ADC_H_

#include"std_types.h"
#include"common_macros.h"
#include"micro_configurations.h"

#define ADC_INTERRUPT
#undef ADC_INTERRUPT  /* Remove This line in case you want to use interrupt */

void ADC_init(void);
uint16 ADC_readChannel(uint8 channel_num);

#ifdef  ADC_INTERRUPT
void ADC_readChannel_INT(uint8 channel_num);
#endif

#endif /* ADC_H_ */
