/*
 * sw_timer.h
 *
 *  Created on: 09 09 2015
 *     
 */

#ifndef SW_TIMER_H_
#define SW_TIMER_H_

#define SwTimerCount  10							// number of sw timer's

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


//=========================================================enum, special for this prj
typedef enum{
		st_u_tim,				// 0_start u delay
		st_i_tim,				// 1_start i delay
		wk_u_tim,				// 2_u work filter
		wk_i_tim,				// 3_i work filter
		lev_1_tim,			// 4_level 1 filter
		lev_2_tim,			// 5_level 2 filter
		lev_err_tim,		// 6_for wrong placement of sensors
		well_in_tim,		// 7_level in well filter
		well_err_tim,		// 8_err in well
		well_hour_tim		// 9_one hour delay
}Timers;
//=========================================================


void SwTimerWork(volatile SW_TIMER* TIMER, unsigned char Count);										//func for tim's processing
void OnSwTimer(volatile SW_TIMER* TIMER, SwTimerMode Mode, unsigned int SwCount);		//preparation of timers
unsigned char GetStatusSwTimer(volatile SW_TIMER* TIMER);														//read tim state
#endif /* SW_TIMER_H_ */
