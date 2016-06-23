/*
 * sw_timer.c
 *
 *  Created on: 09 09 2015
 *
 */
#include "sw_timer.h"




//SW timers subfunction
void SwTimerWork(volatile SW_TIMER* TIMER, unsigned char Count){
	unsigned short i=0;
		for (i=0; i<Count; i++){
			if (TIMER->Mode==SWTIMER_MODE_EMPTY){
				TIMER++;
				continue;
			} 

			if (TIMER->Mode==SWTIMER_MODE_WAIT_ON){ //If timer ON_delay
				if (TIMER->On){
					if (TIMER->LocalCount>0) TIMER->LocalCount--;
					else {
							TIMER->Out=1;
							TIMER->Status=1;
						}
				}//if (TIMER->On)
				else {
					TIMER->Out=0;
					TIMER->LocalCount=TIMER->Count-1;
				}
			}
			
			//-------------------------------------------------------------
			if (TIMER->Mode==SWTIMER_MODE_WAIT_OFF){ //If timer OFF_delay
				if (TIMER->On){
					TIMER->Out=1;
					TIMER->Status=1;
					TIMER->LocalCount=TIMER->Count-1;
				}
				else {
					if (TIMER->LocalCount>0) TIMER->LocalCount--;
						else TIMER->Out=0;
				}
			}
			
			//-------------------------------------------------------------
			if (TIMER->Mode==SWTIMER_MODE_CYCLE){
				if (TIMER->Off){
					if (TIMER->On){
						TIMER->Off=0;
						if (TIMER->LocalCount>0) TIMER->LocalCount--;
					}
				}
				else{
					if (TIMER->LocalCount>0) {
						TIMER->LocalCount--;
						TIMER->Out=0;
					}
					else {
						TIMER->Out=1;
						TIMER->Status=1;
						TIMER->LocalCount=TIMER->Count-1;
					}
				}
				if (TIMER->Reset){
					TIMER->LocalCount=TIMER->Count-1;
					TIMER->Out=0;
					TIMER->Status=0;
				}
			}//if (TIMER->Mode==SWTIMER_MODE_CYCLE)
			
			//---------------------------------------------------------------
			if (TIMER->Mode==SWTIMER_MODE_SINGLE){
				if (TIMER->Off){
					if (TIMER->On){
						TIMER->Off=0;
						if (TIMER->LocalCount>0) TIMER->LocalCount--;
					}
				}
				else{
					if (TIMER->LocalCount>0) {
						TIMER->LocalCount--;
						TIMER->Out=0;
					}
					else {
						TIMER->Out=1;
						TIMER->Status=1;
						TIMER->LocalCount=TIMER->Count-1;
						TIMER->Off=1;
						TIMER->On=0;
					}
				}
				if (TIMER->Reset){
					TIMER->LocalCount=TIMER->Count-1;
					TIMER->Out=0;
					TIMER->Status=0;
				}
			}//if (TIMER->Mode==SWTIMER_MODE_SINGLE)
			
			//------------------------------------------------------------------
			TIMER++;
		}
}

void OnSwTimer(volatile  SW_TIMER* TIMER, SwTimerMode Mode, unsigned int SwCount){
		TIMER->Mode=Mode;
		if (SwCount){
			TIMER->Count=SwCount;
			TIMER->LocalCount=SwCount-1;
		}
		if (TIMER->Mode==SWTIMER_MODE_CYCLE || TIMER->Mode==SWTIMER_MODE_SINGLE){
			TIMER->Off=1;
		}
}


unsigned char GetStatusSwTimer(volatile SW_TIMER* TIMER){
	unsigned char status=0;
	if (TIMER->Mode==SWTIMER_MODE_EMPTY) return -1;
	if (TIMER->Status){
		TIMER->Status=0;
		status=1;
	}
	else {
		status=0;
	}
	return status;
}

