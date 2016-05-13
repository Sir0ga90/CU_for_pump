#ifndef MY_FUNC_H
#define MY_FUNC_H

#include "stm32f030x8.h"
#include "stm32f0xx.h"                  // Device header

//----------------------------------------------------------------------------
typedef enum {_U_, _I_, toggle}Chanel;      		// type of ADC chanel

typedef enum{off, w_rst, i_u}State; 
//----------------------------------------------------------------------------
extern Chanel chanel;														
//----------------------------------------------------------------------------
uint32_t calculate_val(uint32_t adc_val);
uint32_t calculate_val_ac(uint32_t adc_val);

void dig_to_disp(uint32_t out_dig);

uint32_t sample_val_2(uint32_t new_adc);
uint32_t sampl_val_3(Chanel chanel); //10000 counts
inline void store_val(void);
inline void get_real_val(void);

void read_button(void);
void read_button_interrupt(void);
//----------------------------------------------------------------------------

#endif
