
#include "../MCAL/EF_SpechialTIMER.h"
/**************************************************
 * 	global variables
 *************************************************/
static OS_TIMER timers_list[MAX_NUMBER_OF_TIMERS];

/****************************************************************************
* Function    : void_SCHEDULER_timerISRCallback
*
* DESCRIPTION : Function to set the systemTik Flag, "Called in the TIMER ISR"
*
* PARAMETERS  : None
*
* Return Value: None
*
*  NOTE		  : "Don't forget putting Timer_ISR function in the startup code"
******************************************************************************/
static void void_OS_timerISRCallback(void)
{
	//count the enabled timer and if it reached the timeOut ,counter=0 and set timeOut Flag
	EF_void_TimersUpdate();
}

/****************************************************************************
* Function    : void_timerDelete
*
* DESCRIPTION : make all timer_list_parameters = 0
*
* PARAMETERS  : timerNumber
*
* Return Value: None
*
******************************************************************************/
void EF_void_TimerDelete(U8_t timerNumber)
{
	timers_list[timerNumber].counter = 0;
	timers_list[timerNumber].timeOut = 0;
	timers_list[timerNumber].isTimedout = FALSE;
	timers_list[timerNumber].isEnables  =  FALSE;
}

/****************************************************************************
* Function    : void_timerInit
*
* DESCRIPTION : init the timer ,define the callback function and
* 				call void_timerDelete().
*
* PARAMETERS  : None.
*
* Return Value: None.
*
******************************************************************************/
void EF_void_TimerInit(void)
{
	U16_t timersCounter;
	//don't forget put ISR function in startup code
	EF_void_Timer_PORTinit();
	// set the timer interrupt callback
	EF_void_Timer_SetISRCallBack(void_OS_timerISRCallback);
	// set all Timers to the initial state
	for( timersCounter=0 ; timersCounter < MAX_NUMBER_OF_TIMERS ; timersCounter++ )
	{
		EF_void_TimerDelete(timersCounter);
	}
}
/****************************************************************************
* Function    : void_timerInit
*
* DESCRIPTION : set the time out for defined timer.
*
* PARAMETERS  : timerNumber.
* 				timerTimeout.
*
* Return Value: None.
*
******************************************************************************/
void EF_void_TimerCreate(U8_t timerNumber , U16_t timerTimeout) /* Timer timeout in ticks */
{
	if(timers_list[timerNumber].timeOut == 0)
	{
		timers_list[timerNumber].timeOut = timerTimeout;
		timers_list[timerNumber].counter = 0 ;
		timers_list[timerNumber].isTimedout = FALSE ;
		timers_list[timerNumber].isEnables = FALSE;
	}
}
/****************************************************************************
* Function    : void_timersUpdate
*
* DESCRIPTION : count the enabled timer and if it reached the timeOut ,counter=0 and set timeOut Flag
* 				this function called in the ISR by call back function.
*
* PARAMETERS  : None.
*
* Return Value: None.
*
******************************************************************************/
void EF_void_TimersUpdate(void)
{
	U16_t timersCounter=0;

	for( timersCounter=0 ; timersCounter<MAX_NUMBER_OF_TIMERS ; timersCounter++ )
	{
		if(timers_list[timersCounter].isEnables == TRUE)
		{
			timers_list[timersCounter].counter++ ;

			if(timers_list[timersCounter].counter == timers_list[timersCounter].timeOut)
			{
				timers_list[timersCounter].counter = 0;
				timers_list[timersCounter].isTimedout = TRUE;
			}
		}
	}
}
/****************************************************************************
* Function    : bool_timerCheck
*
* DESCRIPTION : check for time out
*
* PARAMETERS  : timerNumber
*
* Return Value: BOOLEAN : return True when timer timeout
*
******************************************************************************/
BOOLEAN EF_BOOLEAN_TimerCheck(U8_t timerNumber)
{
	BOOLEAN timerTimeout = FALSE;
	if(timers_list[timerNumber].isTimedout)
	{
		timers_list[timerNumber].isTimedout = FALSE;
		timerTimeout = TRUE;
	}
	return timerTimeout;
}
/****************************************************************************
* Function    : void_timerStart
*
* DESCRIPTION : enable this timer by making isEnables = True
*
* PARAMETERS  : timerNumber
*
* Return Value: None.
*
******************************************************************************/
void EF_void_TimerStart(U8_t timerNumber)
{
	if(timers_list[timerNumber].timeOut != 0)
	{
		timers_list[timerNumber].isEnables = TRUE;
	}
}
/****************************************************************************
* Function    : void_timerStop
*
* DESCRIPTION : stop this timer by making isEnables = False
*
* PARAMETERS  : timerNumber
*
* Return Value: None.
*
******************************************************************************/
void EF_void_TimerStop(U8_t timerNumber)
{
	timers_list[timerNumber].isEnables = FALSE;
}
/****************************************************************************
* Function    : void_timerReset
*
* DESCRIPTION : make counter = 0
*
* PARAMETERS  : timerNumber
*
* Return Value: None.
*
******************************************************************************/
void EF_void_TimerReset(U8_t timerNumber)
{
	timers_list[timerNumber].counter = 0;
}

