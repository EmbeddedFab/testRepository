/*
 * OS_TIMER_cfg.h
 *
 *  Created on: Jan 29, 2015
 *      Author: MELSOBKY
 * 	File description:
 *      Module Configuration File
 *
 */



/**************************************************
 * configuration types
 *************************************************/
#ifndef OS_TIMER_CFG_H_
#define OS_TIMER_CFG_H_

#include "../ServiceLayer/std_types.h"
#include "EF_TIVA_Timer.h"
/**************************************************
 * predifine variables
 **************************************************/
#define MAX_NUMBER_OF_TASKS    5
#define MAX_NUMBER_OF_TIMERS   10


#define THREE_SEC_TIMER_ID 			0
#define MILLIS_200_TIMER_ID         1
#define TWO_MIN_TIMER_ID			2
#define ONE_SEC_TIMER_ID	   		3
#define ESP8266_FIND_KEYWORD_ID	    4
#define WAIT_FOR_CAN_ID             5
#define UART_TIMER_ID				6
#define SPI_TIMER_ID				7
#define Bluetooth_FIND_KEYWORD_ID	8

#define UART_WAIT_TIME				100
/**************************************************
 * configuration types
 *************************************************/
 
typedef struct timer_struct
{
  volatile U16_t timeOut ;
  volatile U16_t counter ;
  volatile BOOLEAN isTimedout;
  volatile BOOLEAN isEnables;
}OS_TIMER;

#endif
