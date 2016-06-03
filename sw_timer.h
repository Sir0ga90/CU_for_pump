/*
 * sw_timer.h
 *
 *  Created on: 09 09 2015
 *     
 */

#ifndef SW_TIMER_H_
#define SW_TIMER_H_

#define SwTimerCount  2							// number of sw timer's

/*modes*/
typedef enum
	{
		SWTIMER_MODE_EMPTY,
		SWTIMER_MODE_WAIT_ON,
		SWTIMER_MODE_WAIT_OFF,
		SWTIMER_MODE_CYCLE,
		SWTIMER_MODE_SINGLE
} SwTimerMode;


/*sw_tim struct*/
typedef struct
    {
					unsigned LocalCount	:32;		//var for time count
					unsigned Count			:24;		//var for delay
					unsigned Mode				:3;			//mode
					unsigned On					:1;			//start bit
					unsigned Reset			:1;			//reset without disable
					unsigned Off				:1;			//stop bit
					unsigned Out				:1;			//out of timer
 __packed unsigned Status			:1;			//timer state (on/off)
}SW_TIMER;	



void SwTimerWork(volatile SW_TIMER* TIMER, unsigned char Count);										//func for tim's processing
void OnSwTimer(volatile SW_TIMER* TIMER, SwTimerMode Mode, unsigned int SwCount);		//preparation of timers
unsigned char GetStatusSwTimer(volatile SW_TIMER* TIMER);														//read tim state
#endif /* SW_TIMER_H_ */
