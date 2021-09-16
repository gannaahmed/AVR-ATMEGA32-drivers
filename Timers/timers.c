/******************************************************************************
 *
 * Module: Timers
 *
 * File Name: timers.c
 *
 * Description: Source file for timers AVR driver
 *
 * Author: Ganna Ahmed
 *
 *******************************************************************************/

#include "timers.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */

static volatile void (*g_callBackPtr_0)(void) = NULL_PTR;

static volatile void (*g_callBackPtr_1)(void) = NULL_PTR;

static volatile void (*g_callBackPtr_2)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER0_OVF_vect)
{
	if(g_callBackPtr_0 != NULL_PTR)
	{
		/* Call the Call Back function in the application when timer0 overflow occur */
		(*g_callBackPtr_0)();
	}
}

ISR(TIMER0_COMP_vect)
{
	if(g_callBackPtr_0 != NULL_PTR)
	{
		/* Call the Call Back function in the application when timer0 reaches compare value */
		(*g_callBackPtr_0)();
	}
}

ISR(TIMER1_OVF_vect)
{
	if(g_callBackPtr_1 != NULL_PTR)
	{
		/* Call the Call Back function in the application when timer1 overflow occur */
		(*g_callBackPtr_1)();
	}
}

ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtr_1 != NULL_PTR)
	{
		/* Call the Call Back function in the application when timer1 reaches compare value */
		(*g_callBackPtr_1)();
	}
}


ISR(TIMER2_OVF_vect)
{
	if(g_callBackPtr_2 != NULL_PTR)
	{
		/* Call the Call Back function in the application when timer2 overflow occur */
		(*g_callBackPtr_2)();
	}
}

ISR(TIMER2_COMP_vect)
{
	if(g_callBackPtr_2 != NULL_PTR)
	{
		/* Call the Call Back function in the application when timer2 reaches compare value */
		(*g_callBackPtr_2)();
	}
}



/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*************************************************************************************************
 *  [Function Name]:    timer_init
 *  [Description] :		This Function initializes timer0 or timer1 or timer2
 *                      1-Set required Mode (normal or CTC)
 *                      2-Set required prescaler
 *                      3-Set required timer initial value
 *                      4-Set required compare value in case of compare mode
 *  [Args] :            Pointer to Struct timer_ConfigType
 *  [Returns] :			NONE
 ***************************************************************************************************/

void timer_init(const timer_ConfigType * config_Ptr)
{

	if(config_Ptr->s_timerType==0){  /* In case timer0 */

		/************************** TCCR0 Description **************************
		 * FOC0  active in case non-PWM mode
		 * WGM01:0 for timer Mode (00 for normal, 10 for CTC)
		 * COM01:0 in case non-PWM 00 if OC0 disconnected
		 * CS02:0 for clock select
		 ***********************************************************************/
		SET_BIT(TCCR0,FOC0);    /* active for non-PWM mode */

		/* setting required timer mode */
		TCCR0 = (TCCR0 & 0xBF) | (((config_Ptr->s_mode) & 0x01 )<<6);
		TCCR0 = (TCCR0 & 0xF7) | (((config_Ptr->s_mode) & 0x02 )<<2);

		TCCR0 &= ~(1<<COM01) & ~(1<<COM00);  /* non-PWM mode  OC0 disconnected */

		TCCR0 = (TCCR0 & 0xF8) | ((config_Ptr->s_clock01) & 0x07);  /* setting required timer clock */



		TCNT0 = config_Ptr->s_initValue;  /* initializing timer with required initial value  */


		if(config_Ptr->s_mode==0){   /* In case normal mode */

			SET_BIT(TIMSK,TOIE0);  /* Enabling overflow mode interrupt*/
		}

		else if (config_Ptr->s_mode==2){  /* In case compare mode */

			OCR0 = config_Ptr->s_copmareValue;  /* setting required compare value */
			SET_BIT(TIMSK,OCIE0);       /* Enabling compare match interrupt*/
		}

	}


	else if (config_Ptr->s_timerType==2){   /* In case timer2 */

		/************************** TCCR2 Description **************************
		 * FOC2  active in case non-PWM mode
		 * WGM21:0 for timer Mode (00 for normal, 10 for CTC)
		 * COM21:0 in case non-PWM 00 if OC2 disconnected
		 * CS22:0 for clock select
		 ***********************************************************************/

		SET_BIT(TCCR0,FOC2); /* active for non-PWM mode */

		/* setting required timer mode */
		TCCR2 = (TCCR2 & 0xBF) | (((config_Ptr->s_mode) & 0x01 )<<6);
		TCCR2 = (TCCR2 & 0xF7) | (((config_Ptr->s_mode) & 0x02 )<<2);

		TCCR2 &= ~(1<<COM21) & ~(1<<COM20);  /* non-PWM mode  OC2 disconnected */

		TCCR2 = (TCCR2 & 0xF8) | ((config_Ptr->s_clock2) & 0x07);  /* setting required timer clock */



		TCNT2 = config_Ptr->s_initValue;  /* initializing timer with required initial value  */


		if(config_Ptr->s_mode==0){  /* In case normal mode */

			SET_BIT(TIMSK,TOIE2);   /* Enabling overflow mode interrupt*/
		}

		else if (config_Ptr->s_mode==2){  /* In case compare mode */

			OCR2 = config_Ptr->s_copmareValue; /* setting required compare value */
			SET_BIT(TIMSK,OCIE2);  /* Enabling compare match interrupt*/
		}

	}

	else if (config_Ptr->s_timerType==1){

		/************************** TCCR1A Description **************************
		 * COM1A1:0 in case non-PWM 00 if OC1A disconnected
		 * COM1B1:0 in case non-PWM 00 if OC1B disconnected
		 * FOC1A    active in case non-PWM mode
		 * FOC1B    active in case non-PWM mode
		 * WGM11:0  for timer mode
		 ***********************************************************************/

		TCCR1A = 0x0C; /* FOC1A & FOC1B active for non-PWM mode */

		/************************** TCCR1B Description **************************
		 * ICNC1    input capture noise canceler (in case using icu)
		 * ICSE1    input capture edge select (in case using icu)
		 * WGM13:2  for timer mode
		 * CS12:0 for clock select
		 ***********************************************************************/

		TCCR1B &= 0x0F;  /* ICNC1,ICSE1,WGM13 disabled*/

		TCCR1B = (TCCR1B & 0xF8) | ((config_Ptr->s_clock01) & 0x07);  /* setting required timer clock */

		TCNT1 = config_Ptr->s_initValue;  /* initializing timer with required initial value  */


		if(config_Ptr->s_mode==0){  /* In case normal mode */

			CLEAR_BIT(TCCR1B,WGM12); /* setting timer mode to normal */
			SET_BIT(TIMSK,TOIE1);   /* Enabling overflow mode interrupt*/
			}

		else if (config_Ptr->s_mode==2){  /* In case compare mode */

			SET_BIT(TCCR1B,WGM12);  /* setting timer mode to CTC */
			OCR1A = config_Ptr->s_copmareValue;  /* setting required compare value */
			SET_BIT(TIMSK,OCIE1A);   /* Enabling compare match interrupt*/
			}


	}
}





/*************************************************************************************************
 *  [Function Name]:    timer0_setCallBack
 *  [Description] :		This Function stores the address of the function wanted to be called when
 *                      interrupt occur from overflow mode or CTC mode of timer 0
 *  [Args] :            void(*a_ptr_0)(void)
 *                           Address of the function that will be called when receiving interrupt occur
 *  [Returns] :	    	NONE
 ***************************************************************************************************/

void timer0_setCallBack(void(*a_ptr_0)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr_0 = a_ptr_0;
}




/*************************************************************************************************
 *  [Function Name]:    timer1_setCallBack
 *  [Description] :		This Function stores the address of the function wanted to be called when
 *                      interrupt occur from overflow mode or CTC mode of timer 1
 *  [Args] :            void(*a_ptr_1)(void)
 *                           Address of the function that will be called when receiving interrupt occur
 *  [Returns] :	    	NONE
 ***************************************************************************************************/

void timer1_setCallBack(void(*a_ptr_1)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr_1 = a_ptr_1;
}





/*************************************************************************************************
 *  [Function Name]:    timer2_setCallBack
 *  [Description] :		This Function stores the address of the function wanted to be called when
 *                      interrupt occur from overflow mode or CTC mode of timer 2
 *  [Args] :            void(*a_ptr_2)(void)
 *                           Address of the function that will be called when receiving interrupt occur
 *  [Returns] :	    	NONE
 ***************************************************************************************************/

void timer2_setCallBack(void(*a_ptr_2)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr_2 = a_ptr_2;
}





/*************************************************************************************************
 *  [Function Name]:    timer_stop
 *  [Description] :		This Function disables timer0 or timer1 or timer2
 *  [Args] :            Uint8 timer_type
 *                         timer type (0,1,2)
 *  [Returns] :			NONE
 ***************************************************************************************************/

void timer_stop(const uint8 timer_type){

	if(timer_type==0){ /* In case timer 0 */


		CLEAR_BIT(TIMSK,TOIE0); /* Disable timer0 overflow interrupt*/
		SET_BIT(TIMSK,OCIE0);  /* Disable timer0 compare match interrupt*/

		/* Clear All Timer0 Registers */
		TCCR0 = 0;
		TCNT0 = 0;
		OCR0 = 0;
	}

	if(timer_type==2){  /* In case timer 2 */


		CLEAR_BIT(TIMSK,TOIE2);  /* Disable timer2 overflow interrupt*/
		SET_BIT(TIMSK,OCIE2);   /* Disable timer2 compare match interrupt*/

		/* Clear All Timer2 Registers */
		TCCR2 = 0;
		TCNT2 = 0;
		OCR2 = 0;
	}

	if(timer_type==1){  /* In case timer 1 */


		CLEAR_BIT(TIMSK,TOIE1);   /* Disable timer1 overflow interrupt*/
		CLEAR_BIT(TIMSK,OCIE1A);   /* Disable timer1 compare match interrupt*/

		/* Clear All Timer1 Registers */
		TCCR1A = 0;
		TCCR1B = 0;
		TCNT1 = 0;
		OCR1A = 0;
	}



}
