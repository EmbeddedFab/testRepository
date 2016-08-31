/***************************************************************
 *  Source File: EF_TIVA_uart.h
 *
 *  Description: UART driver for TivaC
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

#ifndef UART_H
#define UART_H
#include "../MCAL/EF_TIVA_uart_cfg.h"

void void_UART_Deinit(void);
/****************************************************************************
* Function    : void_UART_init
*
* DESCRIPTION : init the uart
*
* PARAMETERS  : uart_cfg : pointer to UART_cfg_str which has prameters to init the uart
*
* Return Value: None.
*
******************************************************************************/
extern void EF_void_UART_Init(UART_cfg_str *uart_cfg);
/****************************************************************************
* Function    : BOOLEAN_UART_putchar
*
* DESCRIPTION : Sending the TX byte ,used timer to be unStuck
*
* PARAMETERS  : UART_Number : define which uart Module was used
* 				Tx_byte: Tx byte
*
* Return Value: BOOLEAN : return True if Byte is transmitted or false
*
******************************************************************************/
extern BOOLEAN EF_BOOLEAN_UART_PutChar(U8_t UART_Number, U8_t data);
/****************************************************************************
* Function    : BOOLEAN_UART_getchar
*
* DESCRIPTION : Getting the RX byte ,used timer to be unStuck
*
* PARAMETERS  : UART_Number : define which uart Module was used
* 				returnedValue: pointer to the Rx byte
*
* Return Value: BOOLEAN : return True if Byte is recieved or false
*
******************************************************************************/
extern BOOLEAN EF_BOOLEAN_UART_GetChar(U8_t UART_Number, U8_t * returnedValue);
/****************************************************************************
* Function    : u8_UART_waitchar
*
* DESCRIPTION :  Getting the RX byte
*
* PARAMETERS  : UART_Number : define which uart Module was used
*
* Return Value: U8_t : Rx byte
*
******************************************************************************/
extern U8_t EF_u8_UART_WaitChar(U8_t UART_Number);
/****************************************************************************
* Function    : void_UART_putchar
*
* DESCRIPTION :  Sending the TX byte
*
* PARAMETERS  : UART_Number : define which uart Module was used
*				Tx_byte
*
* Return Value: None
*
******************************************************************************/
extern void EF_void_UART_PutChar(U8_t UART_Number, U8_t Tx_byte);


BOOLEAN EF_BOOLEAN_UART_CheckForRxData(U8_t UART_Number);


/****************************************************************************
* Function    : USART_SEND_ARRAY
*
* DESCRIPTION : sending arrays of bytes
*
*
* PARAMETERS  : UART_Number : define which uart Module was used
*				array 		: pointer to TX data
*				Length		: lenght of array
*
* Return Value: None
*
******************************************************************************/
void EF_void_UART_SendArray(U8_t UART_Number, U8_t *array, U8_t Length);


void EF_void_UART_HextoASCII(U8_t UART_Number,U8_t *hex);


#endif
