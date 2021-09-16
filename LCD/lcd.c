 /******************************************************************************
 *
 * [Module]: LCD
 *
 * [File Name]: lcd.c
 *
 * [Description]: Source file for the LCD driver
 *
 * [Author]: Ganna Ahmed
 *
 *******************************************************************************/

#include "lcd.h"

 /******************************************************************************
 *
 * [Function name]: LCD_init
 *
 * [Description]: the function initializes the LCD and configuring it's mode (4bit mode/8bit mode)
 *
 * [Args]: NONE
 *
 * [returns]: NONE
 *
 *******************************************************************************/

void LCD_init(void)
{
	LCD_CTRL_PORT_DIR |= (1<<E) | (1<<RS) | (1<<RW);     /* Configure the control pins(E,RS,RW) as output pins */

	#if (DATA_BITS_MODE == 4)
		#ifdef UPPER_PORT_PINS
			LCD_DATA_PORT_DIR |= 0xF0; 	/* Configure the highest 4 bits of the data port as output pins */
		#else
			LCD_DATA_PORT_DIR |= 0x0F;	 /* Configure the lowest 4 bits of the data port as output pins */
		#endif
		LCD_sendCommand(FOUR_BITS_DATA_MODE);	/* initialize LCD as 4-bit mode */
		LCD_sendCommand(TWO_LINE_LCD_Four_BIT_MODE);     /* use 2-line lcd + 4-bit Data Mode + 5*7 dot display Mode */
	#elif (DATA_BITS_MODE == 8)
		LCD_DATA_PORT_DIR = 0xFF;   /* Configure the data port as output port */
		LCD_sendCommand(TWO_LINE_LCD_Eight_BIT_MODE);   /* use 2-line lcd + 8-bit Data Mode + 5*7 dot display Mode */
	#endif

	LCD_sendCommand(CURSOR_OFF);    /* cursor off */
	LCD_sendCommand(CLEAR_COMMAND);     /* clear LCD at the beginning */
}

/******************************************************************************
*
* [Function name]: LCD_sendCommand
*
* [Description]: the function is used to send a certain command to the LCD
*                from the commands table
*
* [Args]: uint8 command:
*               a hex code from commands table from data sheet
*
* [returns]: NONE
*
*******************************************************************************/

void LCD_sendCommand(uint8 command)
{
	CLEAR_BIT(LCD_CTRL_PORT,RS);	/* selecting command register */
	CLEAR_BIT(LCD_CTRL_PORT,RW);	/* to write data to LCD (RW=0) */
	_delay_ms(1);	/* minimum delay for processing Tas = 50ns (from data sheet) */
	SET_BIT(LCD_CTRL_PORT,E);	 /* Enable LCD display */
	_delay_ms(1);	/* minimum delay for processing Tpw - Tdws = 190ns (from data sheet) */

#if(DATA_BITS_MODE==4)            /* In case 4 bits mode the command will be sent on two steps */
#ifdef UPPER_PORT_PINS
    /* putting the 4 most significant bits firstly (from data sheet) of the command on the upper port pins*/
	LCD_DATA_PORT=(LCD_DATA_PORT& 0x0F)|(command& 0xF0);
#else
	/*putting the 4 most significant bits firstly (from data sheet) of the command on the lower port pins*/
	LCD_DATA_PORT=(LCD_DATA_PORT& 0xF0)|((command& 0xF0)>>4);
#endif
	_delay_ms(1);      /* minimum delay for processing Tdsw = 100ns (from data sheet) */
	CLEAR_BIT(LCD_CTRL_PORT,E);   /* disable LCD display */
	_delay_ms(1);    /* minimum delay for processing Th = 13ns (from data sheet) */
	SET_BIT(LCD_CTRL_PORT,E);  /* Enable LCD display */
	_delay_ms(1);  /* minimum delay for processing Tpw - Tdws = 190ns (from data sheet) */

#ifdef UPPER_PORT_PINS
	 /* putting the 4 least significant bits secondly (from data sheet) of the command on the upper port pins*/
	LCD_DATA_PORT=(LCD_DATA_PORT& 0x0F)|((command& 0x0F)<<4);
#else
	/* putting the 4 least significant bits secondly (from data sheet) of the command on the lower port pins*/
	LCD_DATA_PORT=(LCD_DATA_PORT& 0xF0)|((command& 0x0F));
#endif
	_delay_ms(1);   /* minimum delay for processing Tdsw = 100ns (from data sheet) */
	CLEAR_BIT(LCD_CTRL_PORT,E);  /* disable LCD display */
	_delay_ms(1);   /* minimum delay for processing Th = 13ns (from data sheet) */

#elif(DATA_BITS_MODE==8)    /* In case 8 bits mode the command will be sent on one step */
	LCD_DATA_PORT=command;    /* Sending command on LCD data port */
	_delay_ms(1);             /* minimum delay for processing Tdsw = 100ns (from data sheet) */
	CLEAR_BIT(LCD_CTRL_PORT,E);  /* disable LCD display */
	_delay_ms(1);           /* minimum delay for processing Th = 13ns (from data sheet) */
#endif

}


/******************************************************************************
*
* [Function name]: LCD_displayCharacter
*
* [Description]: the function is used to display one character on the LCD
*
* [Args]: uint8 data:
*               a character that will be displayed on the LCD
*
* [returns]: NONE
*
*******************************************************************************/

void LCD_displayCharacter(uint8 data)
{
	SET_BIT(LCD_CTRL_PORT,RS);	/* selecting data register */
	CLEAR_BIT(LCD_CTRL_PORT,RW);	/* to write data to LCD (RW=0) */
	_delay_ms(1);	/* minimum delay for processing Tas = 50ns (from data sheet) */
	SET_BIT(LCD_CTRL_PORT,E);	 /* Enable LCD display */
	_delay_ms(1);	/* minimum delay for processing Tpw - Tdws = 190ns (from data sheet) */

#if(DATA_BITS_MODE==4)            /* In case 4 bits mode the character will be sent on two steps */
#ifdef UPPER_PORT_PINS
    /* putting the 4 most significant bits firstly (from data sheet) of the character on the upper port pins*/
	LCD_DATA_PORT=(LCD_DATA_PORT& 0x0F)|(data& 0xF0);
#else
	/*putting the 4 most significant bits firstly (from data sheet) of the character on the lower port pins*/
	LCD_DATA_PORT=(LCD_DATA_PORT& 0xF0)|((data& 0xF0)>>4);
#endif
	_delay_ms(1);      /* minimum delay for processing Tdsw = 100ns (from data sheet) */
	CLEAR_BIT(LCD_CTRL_PORT,E);   /* disable LCD display */
	_delay_ms(1);    /* minimum delay for processing Th = 13ns (from data sheet) */
	SET_BIT(LCD_CTRL_PORT,E);  /* Enable LCD display */
	_delay_ms(1);  /* minimum delay for processing Tpw - Tdws = 190ns (from data sheet) */

#ifdef UPPER_PORT_PINS
	 /* putting the 4 least significant bits secondly (from data sheet) of the character on the upper port pins*/
	LCD_DATA_PORT=(LCD_DATA_PORT& 0x0F)|((data& 0x0F)<<4);
#else
	/* putting the 4 least significant bits secondly (from data sheet) of the character on the lower port pins*/
	LCD_DATA_PORT=(LCD_DATA_PORT& 0xF0)|((data& 0x0F));
#endif
	_delay_ms(1);   /* minimum delay for processing Tdsw = 100ns (from data sheet) */
	CLEAR_BIT(LCD_CTRL_PORT,E);  /* disable LCD display */
	_delay_ms(1);   /* minimum delay for processing Th = 13ns (from data sheet) */

#elif(DATA_BITS_MODE==8)    /* In case 8 bits mode the character will be sent on one step */
	LCD_DATA_PORT=data;    /* Sending character on LCD data port */
	_delay_ms(1);             /* minimum delay for processing Tdsw = 100ns (from data sheet) */
	CLEAR_BIT(LCD_CTRL_PORT,E);  /* disable LCD display */
	_delay_ms(1);           /* minimum delay for processing Th = 13ns (from data sheet) */
#endif

}

/******************************************************************************
*
* [Function name]: LCD_displayString
*
* [Description]: the function is used to display a full string
*
* [Args]: char *str:
*               a string that will be displayed on the LCD
*
* [returns]: NONE
*
*******************************************************************************/

void LCD_displayString(const char *str)
{
	/* displaying each character of the string by looping */
	uint8 i;
	for(i=0;str[i]!='\0';i++)
	{
		LCD_displayCharacter(str[i]);
	}
}



/******************************************************************************
*
* [Function name]: LCD_goToRowColumn
*
* [Description]: the function moves the cursor of the LCD to a certain point
*
* [Args]: uint8 row, uint8 col:
*               The row and the column that the function will move the cursor to
*
* [returns]: NONE
*
*******************************************************************************/

void LCD_goToRowColumn(uint8 row,uint8 col){

	uint8 Address;
	/*  first calculating the address of the required position (from data sheet) */
	switch(row)
	{
		case 0:
				Address=col;
				break;
		case 1:
				Address=col+0x40;
				break;
		case 2:
				Address=col+0x10;
				break;
		case 3:
				Address=col+0x50;
				break;
	}

	/* to write to a specific address in the LCD
	 * we need to apply the corresponding command 0b10000000+Address */
	LCD_sendCommand(Address | SET_CURSOR_LOCATION);
}




/******************************************************************************
*
* [Function name]: LCD_displayStringRowColumn
*
* [Description]: the function that displays string on a certain position
*
* [Args]: uint8 row, uint8 col , char *Str :
*               The row and the column that the function will move the cursor to
*               and the string that will be displayed starting from that position
*
* [returns]: NONE
*
*******************************************************************************/
void LCD_displayStringRowColumn(uint8 row,uint8 col,const char *Str)
{
	LCD_goToRowColumn(row,col); /* go to to the required LCD position */
	LCD_displayString(Str); /* display the string */
}


/******************************************************************************
*
* [Function name]: LCD_intgerToString
*
* [Description]: the function displays on the lcd a string given an integer
*
* [Args]: int data
*               the integer that will be displayed as string
*
* [returns]: NONE
*
*******************************************************************************/

void LCD_integerToString(int data)
{
   char buff[16]; /* String to hold the ascii result */
   itoa(data,buff,10); /* 10 for decimal */
   LCD_displayString(buff);
}


/******************************************************************************
*
* [Function name]: LCD_clearScreen
*
* [Description]: the function that clears the LCD screen
*
* [Args]: NONE
*
* [returns]: NONE
*
*******************************************************************************/
void LCD_clearScreen(void)
{
	LCD_sendCommand(CLEAR_COMMAND); /*clear display screen*/
}


