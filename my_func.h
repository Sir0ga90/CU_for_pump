#ifndef MY_FUNC_H
#define MY_FUNC_H

#include "error.h"
//----------------------------------------------------------------------------

typedef enum{
	_U_,
	_I_, 
	toggle
}Chanel;      		// type of ADC chanel

typedef enum{
	off,
	w_rst,
	i_u
}Butt_state; 

//----------------------------------------------------------------------------

extern Chanel chanel;														

//----------------------------------------------------------------------------

uint32_t calculate_val(uint32_t adc_val);
uint32_t calculate_val_ac(uint32_t adc_val);

void dig_to_disp(uint32_t out_dig, Val_on_disp *);

uint32_t sample_val_2(uint32_t new_adc);
uint32_t sampl_val_3(Chanel chanel); //10000 counts
inline void store_val(void);
inline void get_real_val(void);

void read_button(void);
void read_button_interrupt(void);
void set_but_vals(void);

//-----------------------------------------------------------------------------Extern_varaibles
extern uint32_t I_accum;
extern uint32_t U_accum;
extern uint32_t adc_val[2];
extern uint32_t I_val;
extern uint32_t U_val;
extern uint32_t cnt;
extern Butt_state button;

extern uint8_t push_flag;

extern Chanel chanel;

extern Error error_type;


#endif
