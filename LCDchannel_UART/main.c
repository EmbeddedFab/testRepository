/*
 * main.c
 *
 *  Created on: ??þ/??þ/????
 *      Author: Kareem
 */


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>

#include "HAL/EF_LCD.h"
#include "MCAL/EF_UART.h"
#include "MCAL/EF_SpechialTimer.h"

int main (void)
{

	EF_void_LCD_init();
	EF_void_LCD_Clear_Screen();
	EF_void_LCD_goto( 1, 1);

	EF_void_LCD_send_data('a');
	_delay_ms(1000);


     	while(1)
     	{

     	}

}



