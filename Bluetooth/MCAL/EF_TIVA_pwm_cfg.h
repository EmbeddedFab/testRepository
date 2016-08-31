/***************************************************************
 *  Source File: EF_TIVA_PWM_cfg.h
 *
 *  Description: PWM driver for TIVAC
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
#ifndef TIVA_PWM_CFG_H_
#define TIVA_PWM_CFG_H_

#include "std_types.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "../ServiceLayer/std_types.h"
#include "driverlib/pin_map.h"
#include "driverlib/debug.h"
#include "inc/hw_gpio.h"
#include "driverlib/interrupt.h"
#include "inc/hw_ints.h"
#include "driverlib/pwm.h"


/**************************************************
 * Predefine variables
 **************************************************/
typedef enum
{
	M0PWM0 = 0, 
	M0PWM1,
	M0PWM2,
	M0PWM3,
	M1PWM6,
	M1PWM7
}TenumPWM_Ports;

 typedef struct
{
	TenumPWM_Ports PWM_Number;
	/*64 pre-scaler is used , so Fpwm will be from 9HZ to 625KHZ, if you don't want this ,change the pre-scaler in macro and in function init
	  to make Fpwm the same as the Atmega project ,make it 31KHZ*/
	U32_t PWM_frequency;
	U8_t DutyCycle; /* 0 to 100 , percentage */
}PWM_cfg_str;

#endif
