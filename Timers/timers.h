/******************************************************************************
 *
 * Module: Timers
 *
 * File Name: timers.h
 *
 * Description: timers header file
 *
 * Author: Ganna Ahmed
 *
 *******************************************************************************/

#ifndef TIMERS_H_
#define TIMERS_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_configurations.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

typedef enum
{
	normal,compare=2
}timer_mode;

typedef enum{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}timer_01_clock;

typedef enum{
	NO_CLOCK_2,F_CPU_CLOCK_2,F_CPU_8_2,F_CPU_32_2,F_CPU_64_2,F_CPU_128_2,F_CPU_256_2,F_CPU_1024_2
}timer_2_clock;

typedef struct
{
	uint8 s_timerType;
	timer_mode s_mode;
	uint16 s_initValue;
	timer_01_clock s_clock01;
	timer_2_clock s_clock2;
	uint16 s_copmareValue;
}timer_ConfigType;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/* Function initializes timer0 or timer1 or timer2
 *  1-Set required Mode (normal or CTC)
 *  2-Set required prescaler
 *  3-Set required timer initial value
 *  4-Set required compare value in case of compare mode */
void timer_init(const timer_ConfigType * config_Ptr);

/* Function stores the address of the function wanted to be called when interrupt occur from overflow mode or CTC mode of timer 0 */
void timer0_setCallBack(void(*a_ptr_0)(void));

/* Function stores the address of the function wanted to be called when interrupt occur from overflow mode or CTC mode of timer 1 */
void timer1_setCallBack(void(*a_ptr_1)(void));

/* Function stores the address of the function wanted to be called when interrupt occur from overflow mode or CTC mode of timer 2 */
void timer2_setCallBack(void(*a_ptr_2)(void));

/* Function disables timer0 or timer1 or timer2 */
void timer_stop(const uint8 timer_type);



#endif /* TIMERS_H_ */
