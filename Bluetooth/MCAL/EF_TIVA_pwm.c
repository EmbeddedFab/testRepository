/***************************************************************
 *  Source File: EF_TIVA_PWM.c
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
#include "EF_TIVA_pwm.h"
	/*64 pre-scaler is used , so Fpwm will be from 9HZ to 625KHZ, if you don't want this ,
	change the pre-scaler in this macro and in function init at [ SysCtlPWMClockSet( SYSCTL_PWMDIV_64);] */

#define PWM_PRESCALE 64
#define NUMBER_OF_CHOSSEN_PWMs 6
/*
	1- M0PWM0 .... PB6
	2- M0PWM1 .... PB7
	3- M0PWM2 .... PB4
	4- M0PWM3 .... PB5
	5- M1PWM6 .... PF2  >>M1
	6- M1PWM7 .... PF3  >>M1 
*/


typedef enum
{
	CLC_GPIO_ELEMENT ,CLC_PWM_ELEMENT,GPIO_BASE_ELEMENT, PWM_BASE_ELEMENT, GPIO_ELEMENT, MODULE_ELEMENT, GEN_ELEMENT, ADD_ELEMENT , ID_ELEMENT
}TenumPWM_Array_Elements;
const  U32_t PWM_PinConfiguration_Array [NUMBER_OF_CHOSSEN_PWMs][9] =
	{{ SYSCTL_PERIPH_GPIOB, SYSCTL_PERIPH_PWM0, GPIO_PORTB_BASE, PWM0_BASE, GPIO_PIN_6, GPIO_PB6_M0PWM0, PWM_GEN_0, PWM_OUT_0, PWM_OUT_0_BIT},
	 { SYSCTL_PERIPH_GPIOB, SYSCTL_PERIPH_PWM0, GPIO_PORTB_BASE, PWM0_BASE, GPIO_PIN_7, GPIO_PB7_M0PWM1, PWM_GEN_0, PWM_OUT_1, PWM_OUT_1_BIT},
	 { SYSCTL_PERIPH_GPIOB, SYSCTL_PERIPH_PWM0, GPIO_PORTB_BASE, PWM0_BASE, GPIO_PIN_4, GPIO_PB4_M0PWM2, PWM_GEN_1, PWM_OUT_2, PWM_OUT_2_BIT},
	 { SYSCTL_PERIPH_GPIOB, SYSCTL_PERIPH_PWM0, GPIO_PORTB_BASE, PWM0_BASE, GPIO_PIN_5, GPIO_PB5_M0PWM3, PWM_GEN_1, PWM_OUT_3, PWM_OUT_3_BIT},
	 { SYSCTL_PERIPH_GPIOF, SYSCTL_PERIPH_PWM1, GPIO_PORTF_BASE, PWM1_BASE, GPIO_PIN_2, GPIO_PF2_M1PWM6, PWM_GEN_3, PWM_OUT_6, PWM_OUT_6_BIT},
	 { SYSCTL_PERIPH_GPIOF, SYSCTL_PERIPH_PWM1, GPIO_PORTF_BASE, PWM1_BASE, GPIO_PIN_3, GPIO_PF3_M1PWM7, PWM_GEN_3, PWM_OUT_7, PWM_OUT_7_BIT}
	 };


/*********************************************************************
* Function    : EF_void_PWM_init(U8_t u8TimerNumber);
*
* DESCRIPTION : Initializes the PWM in fast mode.
*
* PARAMETERS  : The number of 8 bit timer to set for fast mode pwm.
*               Either Timer 0 or Timer 2.
*
* Return Value: Nothing
***********************************************************************/
void EF_void_PWM_init(PWM_cfg_str *PWM_cfg)
{
	SysCtlPeripheralEnable(PWM_PinConfiguration_Array [PWM_cfg-> PWM_Number][CLC_GPIO_ELEMENT]);
	SysCtlPeripheralEnable(PWM_PinConfiguration_Array [PWM_cfg-> PWM_Number][CLC_PWM_ELEMENT]);
	GPIOPinConfigure(PWM_PinConfiguration_Array [PWM_cfg-> PWM_Number][MODULE_ELEMENT]);
	GPIOPinTypePWM  (PWM_PinConfiguration_Array [PWM_cfg-> PWM_Number][GPIO_BASE_ELEMENT] ,
					 PWM_PinConfiguration_Array [PWM_cfg-> PWM_Number][GPIO_ELEMENT] );
	//prescaler = F/64
	SysCtlPWMClockSet( SYSCTL_PWMDIV_64);
	PWMGenConfigure(PWM_PinConfiguration_Array [PWM_cfg-> PWM_Number][PWM_BASE_ELEMENT],
					PWM_PinConfiguration_Array [PWM_cfg-> PWM_Number][GEN_ELEMENT], PWM_GEN_MODE_DOWN);
	
}


/****************************************************************************
* Function    : EF_void_PWM_SetDutyCycle(U8_t DutyCycle, U8_t u8TimerNumber)
*
* DESCRIPTION : Sets duty cycle for the  PWM
*
* PARAMETER 1 : The duty cycle value. A value between (0 %) and (100 %).
*
* PARAMETER 2 : The number of 8 bit timer. Either Timer 0 or Timer 2.
*
* Return Value: Nothing
******************************************************************************/
void EF_void_PWM_SetDutyCycle(PWM_cfg_str *PWM_cfg)
{
	U32_t ui32Load = 0;
	ui32Load = ( SysCtlClockGet() )/ ((PWM_cfg-> PWM_frequency) * PWM_PRESCALE) ;
	PWMGenPeriodSet(PWM_PinConfiguration_Array [PWM_cfg-> PWM_Number][PWM_BASE_ELEMENT],
					PWM_PinConfiguration_Array [PWM_cfg-> PWM_Number][GEN_ELEMENT] 
					, ui32Load );
	// Encoded offset address of PWM5
	PWMPulseWidthSet (PWM_PinConfiguration_Array [PWM_cfg-> PWM_Number][PWM_BASE_ELEMENT],
					  PWM_PinConfiguration_Array [PWM_cfg-> PWM_Number][ADD_ELEMENT],
					  ( (PWM_cfg-> DutyCycle)* ui32Load)/100);
	// Bit-wise ID for PWM5
	PWMOutputState(PWM_PinConfiguration_Array [PWM_cfg-> PWM_Number][PWM_BASE_ELEMENT],
				   PWM_PinConfiguration_Array [PWM_cfg-> PWM_Number][ID_ELEMENT],
				   true);
	PWMGenEnable(PWM_PinConfiguration_Array [PWM_cfg-> PWM_Number][PWM_BASE_ELEMENT],	
				 PWM_PinConfiguration_Array [PWM_cfg-> PWM_Number][GEN_ELEMENT]);
}


