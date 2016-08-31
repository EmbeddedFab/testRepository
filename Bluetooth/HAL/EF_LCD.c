/***************************************************************
 *  Source File: EF_LCD.c
 *
 *  Description: This simple driver for LCD 2X16 4bit mode
 *
 *  History:     Version 1.0  - INIT Version
 *  Date   :     25/08/2014
 *  -------------------------------------------------------
 *  Author :     EmbeddedFab.
 *
 *  Copyright (C) <2014>  <EmbeddedFab>
 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 
 **************************************************************/
/**************************************************************
 * Notes: This driver can use with 4 bit mode only.
 *
 **************************************************************/


#include <string.h>
#include "utils/ustdlib.h"
#include "EF_LCD.h"
#include "../MCAL/EF_TIVA_DIO.h"

/**************************************************************
 * Definitions
 **************************************************************/


#define LCD_DATA0_PORT   'a'     /**< port for 4bit data bit 0 */
#define LCD_DATA0_PIN     4      /**< pin for 4bit data bit 0  */

#define LCD_DATA1_PORT   'a'     /**< port for 4bit data bit 1 */
#define LCD_DATA1_PIN     3      /**< pin for 4bit data bit 1  */

#define LCD_DATA2_PORT   'f'     /**< port for 4bit data bit 2 */
#define LCD_DATA2_PIN     4      /**< pin for 4bit data bit 2  */

#define LCD_DATA3_PORT   'a'     /**< port for 4bit data bit 3 */
#define LCD_DATA3_PIN     2      /**< pin for 4bit data bit 3  */

#define LCD_RS_PORT	     'e'
#define LCD_RS_PIN		  2

#define LCD_ENABLE_PORT	 'f'
#define LCD_ENABLE_PIN	  1


/**************************************************************
 * API Functions
 **************************************************************/


/*********************************************************************
* Function    :  itoa_Convert (U16_t Number , U8_t * NumberASCII_ptr)
*
* DESCRIPTION : This function used to convert integer number to ASCII
*
* PARAMETERS  : U16_t Number : Number ,max. digits of it should be equalled  MAX_DIGIT_FOR_ITOA
* 				U8_t * NumberASCII_ptr [MAX_DIGIT_FOR_ITOA] : pointer to the ASCII number after Conversion
*
* Return Value: BOOLEAN to check for Errors
**********************************************************************/
/* Enter Number of MAX_DIGIT */
static void itoa_Convert (U32_t Number , U8_t * NumberASCII_ptr)
{
	U8_t  ArrayIndex = 0;
	U8_t NumberOfDigits = 0;
	U32_t Number_temp = Number;
	while ( Number == 0 )
	{
		NumberASCII_ptr[ArrayIndex] = '0';
		return;
	}
	while ( Number_temp > 0 )
	{
		Number_temp = Number_temp/10;
		NumberOfDigits++;
	}
	for ( ArrayIndex =0 ; ArrayIndex < NumberOfDigits ; ArrayIndex++)
	{
		NumberASCII_ptr[ArrayIndex] = Number %10 +'0';
		Number = Number/10;
	}
	/* rotate , using Number as a temp */
	for ( ArrayIndex =0 ; ArrayIndex< NumberOfDigits/2 ; ArrayIndex++)
	{
		Number     = NumberASCII_ptr[ArrayIndex];
		NumberASCII_ptr[ArrayIndex]   = NumberASCII_ptr[NumberOfDigits-ArrayIndex-1];
		NumberASCII_ptr[NumberOfDigits-ArrayIndex-1] = Number;
	}
}


/*********************************************************************
 * Function    : void initLCD(void);
 *
 * DESCRIPTION : This function used to Initialize LCD Ports and send suitable default
 *               CMD before start display.
 *
 * PARAMETERS  : Void.
 *
 * Return Value: Void.
 ***********************************************************************/
void EF_void_LCD_init()
{
	/* Init Control PIN as Output */
	 EF_S8_t_DIO_InitPin ( LCD_RS_PORT , LCD_RS_PIN , OUTPUT);
	 EF_S8_t_DIO_InitPin ( LCD_ENABLE_PORT , LCD_ENABLE_PIN , OUTPUT);
	 /* make the o/p on EN and RS = 0 */
	 EF_S8_DIO_ClearPin (LCD_RS_PORT, LCD_RS_PIN );
	 EF_S8_DIO_ClearPin (LCD_ENABLE_PORT , LCD_ENABLE_PIN);

	/* Init Data PIN as Output 4,5,6,7 */
	 EF_S8_t_DIO_InitPin (LCD_DATA0_PORT , LCD_DATA0_PIN, OUTPUT );
	 EF_S8_t_DIO_InitPin (LCD_DATA1_PORT , LCD_DATA1_PIN, OUTPUT );
	 EF_S8_t_DIO_InitPin (LCD_DATA2_PORT , LCD_DATA2_PIN, OUTPUT );
	 EF_S8_t_DIO_InitPin (LCD_DATA3_PORT , LCD_DATA3_PIN, OUTPUT );

	/* after power up , delay 10ms should be existed */
	_delay_ms(10);
	EF_void_LCD_send_command(0x28);            /* Command ..0x28---> 4-bit mode - 2 line - 5x7 font */
	EF_void_LCD_send_command(0x0C);            /* Command ..0x0C---> Display no cursor - no blink */
	EF_void_LCD_send_command(0x06);            /* Command ..0x06---> Automatic Increment - No Display shift */
	EF_void_LCD_send_command(0x80);            /* Command ..0x80---> Address DDRAM with 0 offset 80h */
	_delay_ms(20);
}


/*********************************************************************
 * Function    : void EF_EF_void_LCD_send_command(U8_t cmnd);
 *
 * DESCRIPTION : This function used to send CMD to LCD like move or control
 *               display mode.
 *
 * PARAMETERS  : U8_t cmnd:  Command User want to send.
 *
 * Return Value: Void.
 ***********************************************************************/
void EF_void_LCD_send_command(U8_t cmnd)
{
	/*RS will be LOW */
	 EF_S8_DIO_ClearPin (LCD_RS_PORT, LCD_RS_PIN );

	 /*
	 * output high nibble first , "Generic method", attach data by bit
	 * first: put 0 in each pin then if the corresponding bit =1 , put 1
	 * -------- output high nibble first ---------*\
	 * * */
	 EF_S8_DIO_ClearPin (LCD_DATA3_PORT, LCD_DATA3_PIN );
	 EF_S8_DIO_ClearPin (LCD_DATA2_PORT, LCD_DATA2_PIN );
	 EF_S8_DIO_ClearPin (LCD_DATA1_PORT, LCD_DATA1_PIN );
	 EF_S8_DIO_ClearPin (LCD_DATA0_PORT, LCD_DATA0_PIN );

	if(cmnd & 0x80)  EF_S8_DIO_SetPin (LCD_DATA3_PORT, LCD_DATA3_PIN );
	if(cmnd & 0x40)  EF_S8_DIO_SetPin (LCD_DATA2_PORT, LCD_DATA2_PIN );
	if(cmnd & 0x20)  EF_S8_DIO_SetPin (LCD_DATA1_PORT, LCD_DATA1_PIN );
	if(cmnd & 0x10)  EF_S8_DIO_SetPin (LCD_DATA0_PORT, LCD_DATA0_PIN );

	/* make Enable High to low with delay to enable the internal latch at the LCD*/
	EF_S8_DIO_SetPin (LCD_ENABLE_PORT , LCD_ENABLE_PIN);
	_delay_us(2);
	EF_S8_DIO_ClearPin (LCD_ENABLE_PORT , LCD_ENABLE_PIN);

	/*------------ output low nibble--------------- */
	 EF_S8_DIO_ClearPin (LCD_DATA3_PORT, LCD_DATA3_PIN );
	 EF_S8_DIO_ClearPin (LCD_DATA2_PORT, LCD_DATA2_PIN );
	 EF_S8_DIO_ClearPin (LCD_DATA1_PORT, LCD_DATA1_PIN );
	 EF_S8_DIO_ClearPin (LCD_DATA0_PORT, LCD_DATA0_PIN );

	if(cmnd & 0x08)  EF_S8_DIO_SetPin (LCD_DATA3_PORT, LCD_DATA3_PIN );
	if(cmnd & 0x04)  EF_S8_DIO_SetPin (LCD_DATA2_PORT, LCD_DATA2_PIN );
	if(cmnd & 0x02)  EF_S8_DIO_SetPin (LCD_DATA1_PORT, LCD_DATA1_PIN );
	if(cmnd & 0x01)  EF_S8_DIO_SetPin (LCD_DATA0_PORT, LCD_DATA0_PIN );

	/* make Enable High to low with delay to enable the internal latch at the LCD*/
	EF_S8_DIO_SetPin (LCD_ENABLE_PORT , LCD_ENABLE_PIN);
	_delay_us(2);
	EF_S8_DIO_ClearPin (LCD_ENABLE_PORT , LCD_ENABLE_PIN);

	/* wait to let LCD running the command*/
	_delay_us(300);
}

/*********************************************************************
 * Function    : void EF_void_LCD_send_data(U8_t data);
 *
 * DESCRIPTION : This function used to send data to LCD .
 *
 * PARAMETERS  : U8_t Data:  Data User want to send.
 *
 * Return Value: Void.
 ***********************************************************************/
void EF_void_LCD_send_data(char data)
{
	/* RS will be high */
	EF_S8_DIO_SetPin (LCD_RS_PORT, LCD_RS_PIN );

	 /*
	 * output high nibble first , "Generic method", attach data by bit
	 * first: put 0 in each pin then if the corresponding bit =1 , put 1
	 * -------- output high nibble first ---------*\
	 * * */
	 EF_S8_DIO_ClearPin (LCD_DATA3_PORT, LCD_DATA3_PIN );
	 EF_S8_DIO_ClearPin (LCD_DATA2_PORT, LCD_DATA2_PIN );
	 EF_S8_DIO_ClearPin (LCD_DATA1_PORT, LCD_DATA1_PIN );
	 EF_S8_DIO_ClearPin (LCD_DATA0_PORT, LCD_DATA0_PIN );

	if(data & 0x80)  EF_S8_DIO_SetPin (LCD_DATA3_PORT, LCD_DATA3_PIN );
	if(data & 0x40)  EF_S8_DIO_SetPin (LCD_DATA2_PORT, LCD_DATA2_PIN );
	if(data & 0x20)  EF_S8_DIO_SetPin (LCD_DATA1_PORT, LCD_DATA1_PIN );
	if(data & 0x10)  EF_S8_DIO_SetPin (LCD_DATA0_PORT, LCD_DATA0_PIN );

	/* make Enable High to low with delay to enable the internal latch at the LCD*/
	EF_S8_DIO_SetPin (LCD_ENABLE_PORT , LCD_ENABLE_PIN);
	_delay_us(2);
	EF_S8_DIO_ClearPin (LCD_ENABLE_PORT , LCD_ENABLE_PIN);

	/*------------ output low nibble--------------- */
	 EF_S8_DIO_ClearPin (LCD_DATA3_PORT, LCD_DATA3_PIN );
	 EF_S8_DIO_ClearPin (LCD_DATA2_PORT, LCD_DATA2_PIN );
	 EF_S8_DIO_ClearPin (LCD_DATA1_PORT, LCD_DATA1_PIN );
	 EF_S8_DIO_ClearPin (LCD_DATA0_PORT, LCD_DATA0_PIN );

	if(data & 0x08)  EF_S8_DIO_SetPin (LCD_DATA3_PORT, LCD_DATA3_PIN );
	if(data & 0x04)  EF_S8_DIO_SetPin (LCD_DATA2_PORT, LCD_DATA2_PIN );
	if(data & 0x02)  EF_S8_DIO_SetPin (LCD_DATA1_PORT, LCD_DATA1_PIN );
	if(data & 0x01)  EF_S8_DIO_SetPin (LCD_DATA0_PORT, LCD_DATA0_PIN );

	/* make Enable High to low with delay to enable the internal latch at the LCD*/
	EF_S8_DIO_SetPin (LCD_ENABLE_PORT , LCD_ENABLE_PIN);
	_delay_us(2);
	EF_S8_DIO_ClearPin (LCD_ENABLE_PORT , LCD_ENABLE_PIN);

	/* wait to let LCD running the command*/
	_delay_us(300);
}

/***********************************************************************
 * Function    : void EF_void_LCD_goto(U8_t y, U8_t x);
 *
 * DESCRIPTION : This function moves the cursor the line y column x
 *               on the LCD module.
 *
 * PARAMETERS  : U8_t y, U8_t x.
 *
 * Return Value: Void.
 ***********************************************************************/
void EF_void_LCD_goto(U8_t y, U8_t x)
{
	U8_t firstAddress[] = {0x80,0xC0,0x94,0xD4};

	EF_void_LCD_send_command(firstAddress[y-1] + x-1);
	_delay_ms(10);
}

/***********************************************************************
* Function    : EF_void_LCD_print_NUM(S16_t s16Numberstring,U8_t u8ColNumber);
*
* DESCRIPTION : This function used to convert integer number to ASCII
* 				then print it in LCD
*
* PARAMETERS  : s16Numberstring: integer number wanted to be displayed in LCD
*				u8RowNumber    : to define which row to display ,
*				take 1 for first row, 2: for the second row
*
* Return Value: Void.
***********************************************************************/
void EF_void_LCD_print_NUM(U32_t u16Numberstring,U8_t u8RowNumber)
{
	U8_t ArrayIndex = 0;
	U8_t s8DisplayStr[6] = {0};
	/* convert number to ASCII */
	itoa_Convert(u16Numberstring, s8DisplayStr);
	for(ArrayIndex = 0; s8DisplayStr[ArrayIndex] != 0; ArrayIndex++)
	{
		EF_void_LCD_goto(u8RowNumber, (ArrayIndex + 1));
		EF_void_LCD_send_data(s8DisplayStr[ArrayIndex]);
	}
}

/***********************************************************************
* Function    : void EF_void_LCD_print(U8_t *string);
*
* DESCRIPTION : This function used to display String which user
*               wants to display .
*
* PARAMETERS  : U8_t *string:  String User want to send.
*
* Return Value: Void.
***********************************************************************/
void EF_void_LCD_print(char *string)
{
	U8_t i = 0;

	while(string[i]!=0)
	{
		EF_void_LCD_send_data(string[i]);
		i++;
	}
}

/***********************************************************************
* Function    : void EF_void_LCD_print_ByLength
*
* DESCRIPTION : This function used to display String which user
*               wants to display .
*
* PARAMETERS  : U8_t *string:  String User want to send.
* 				U8_t  length:  number of characters to be displayed
*
* Return Value: Void.
***********************************************************************/
void EF_void_LCD_print_ByLength(unsigned char *string, U8_t length)
{
	U8_t i = 0;

	while( i < length)
	{
		EF_void_LCD_send_data(string[i]);
		i++;
	}
}

/***********************************************************************
* Function    : EF_void_LCD_Clear_Screen(void);
*
* DESCRIPTION : This function used to clear the LCD screen
*
* PARAMETERS  : Void.
*
* Return Value: Void.
***********************************************************************/
void EF_void_LCD_Clear_Screen(void)
{
	EF_void_LCD_send_command(1<<0);
	_delay_us(2);
	_delay_ms(10);
}

/***********************************************************************
* Function    :EF_void_uploadCustomChar(U8_t location, unsigned char *character);
*
* DESCRIPTION : This function used to upload a custom char which user wants to map
* 				it using CGRAM (Character Generator Ram Address set).
*
* PARAMETERS  : location : to define which CGRAM ADD offset. It takes from (0 to 7).
* 				character: pointer to 8 pointer to char as CGRAM needs 8 char to map
*
* Return Value: Void.
***********************************************************************/
void EF_void_uploadCustomChar(U8_t location, unsigned char *character)

{
	U8_t ArrayIndex;
	/* the Add offset must be 0 to 7*/
	if (location < 8)
	{
		EF_void_LCD_send_command(0x40 + (location * 8));
		for (ArrayIndex = 0; ArrayIndex < 8; ArrayIndex++)
		{
			EF_void_LCD_send_data(character[ArrayIndex]);
		}
	}
	else
	{
		/* TODO: return Error */
	}

}


/***********************************************************************
* Function    : EF_void_LCD_HextoASCII(U8_t *hex);
*
* DESCRIPTION : This function used to convert hex to ASCII, then display it
*
* PARAMETERS  : hex: pointer to hex (one byte)
*
* Return Value: Void.
***********************************************************************/
void EF_void_LCD_HextoASCII(U8_t *hex)
{

	U8_t temp_L = 0;                    // dummy variable that will hold LSBs of Hex number
	U8_t temp_H = 0;                    // dummy variable that will hold MSBs of Hex number

	temp_H = *hex & 0xF0;           // Obtain the upper 4 bits (MSBs) of hex number
	temp_H = temp_H >> 4;           //

	if(temp_H >9)                   // Check if the number is a letter
	{
		temp_H += 0x37;             // Convert the number to a letter in ASCII
	}
	else
	{
		temp_H += 0x30;             // Convert the number to ASCII number
	}

	EF_void_LCD_send_data(temp_H);           // Display the number

	temp_L = *hex & 0x0F;           // Obtain the lower 4 bits (LSBs) of hex number

	if(temp_L >9)                   // Check if the the number is a letter
	{
		temp_L += 0x37;             // Convert the number to a letter in ASCII
	}
	else
	{
		temp_L += 0x30;             // Convert the number to ASCII number
	}

	EF_void_LCD_send_data(temp_L);           // Display the number
}

