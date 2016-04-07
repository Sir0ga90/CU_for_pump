#include "stm32f030x8.h"
#include "stm32f0xx.h"                  // Device header




typedef enum {_I_, _U_, toggle}Chanel;      // type of ADC input

uint32_t calculate_val(uint32_t adc_val);

void dig_to_disp(uint32_t out_dig);

uint32_t sample_val_2(uint32_t new_adc);

void read_button(void);

extern Chanel chanel;
extern uint8_t button;
