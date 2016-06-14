#include "my_func.h"
#include "stm32f030x8.h"
#include "stm32f0xx.h"                  // Device header
#include "blink.h"
#include "math.h"
#include "level.h"
//------------------------------------------------------------------------------------------------------------------------------------Global_varaibles

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint32_t calculate_val(uint32_t adc_val){														//return real val for 3.3V meter
	
	static const uint16_t devide_coef = 1220;													// coefficient for convertion, practical val (calculate val - 4095/330 = 12,41)
	return (adc_val*100)/devide_coef;																	// F.E. for 12 bit adc for 4095 = 3.3V: 4095*100/1220, 
																																		// "*100" for flush val in 3 seg disp
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
static uint8_t counter = 0;		// counter for sigments of led: for 1 interrupt 1 segment
static uint32_t tmp = 0;
static uint32_t dig = 0;
static uint8_t dp = 0;
	
void dig_to_disp(uint32_t out_dig, Val_on_disp *odl_f){
	switch (counter){
		case 0:
			tmp = out_dig;
			dig = tmp%10;			
			tmp = tmp/10;
			init_disp();
			on_seg_3();
			dig_to_port(dig, dp);
			break;
		case 1:
			dig = tmp%10;
			tmp = tmp/10;
			init_disp();
			on_seg_2();
			
			if ( (error_type == E_I || error_type == ELI || chanel == _I_) && *odl_f == e_cnl){
				dp = 1;
			}
				
			dig_to_port(dig, dp);															
			break;
		case 2:
			dig = tmp%10;
			init_disp();
			on_seg_1();
			if (*odl_f == e_r_timer) dp = 1;
			dig_to_port(tmp, dp);
			dp = 0;
			break;
	}
	dp = 0;
	counter++;
	if (counter > 2) counter = 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
uint32_t sample_val_2(uint32_t new_adc){
	uint8_t i = 0;
	uint32_t disp_val = 0;
	uint32_t last_val = 0;
	
	disp_val += new_adc;
	
	if (i > 99){
		disp_val /= 100;
		last_val = disp_val;
	}
	i++;
	return last_val;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void read_button_interrupt(void){
	HAL_Delay(20);
	switch(button){
		case w_rst:{
			chanel++;
			if (chanel == toggle){ 
				chanel = _I_;
			}
			button = off;
			break;
			}
		default:{
			button = off;
			break;
		}
	}
			
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void read_button(void){
	
	if (push_flag == RESET){																								// if button was not pushed before
		
		if ((GPIOA->IDR & but_i_u_Pin) || (GPIOA->IDR & but_w_rst_Pin)){ 			// if one of the button's is pushed
			HAL_Delay(5);																												// first debounced 	delay	
			if ((GPIOA->IDR & but_i_u_Pin) || (GPIOA->IDR & but_w_rst_Pin)){		// if pushed so far
				HAL_Delay(15);																										// second debounced delay
				if (HAL_GPIO_ReadPin(but_i_u_GPIO_Port, but_i_u_Pin) == RESET){
					button = i_u;
					push_flag = SET;
				}
				else if (HAL_GPIO_ReadPin(but_w_rst_GPIO_Port, but_w_rst_Pin) == RESET){
					button = w_rst;
					push_flag = SET;
				}
				else
					button = off;
				
			}
			else 
				button = off;																											// if was button bounce
	
		}
		else
			button = off;																												// if was button bounce
	}
	
	if ((GPIOA->IDR & (but_i_u_Pin | but_w_rst_Pin)) == (but_i_u_Pin | but_w_rst_Pin)){ // if button's released after pushing
		push_flag = RESET;
	}
	
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void set_but_vals(void){
		
	if (button == i_u){
		chanel++;		
		if (chanel == toggle) chanel = _U_;
	}
	
	button = off;
	
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
uint32_t sampl_val_3(Chanel chanel){ //unused
	
	uint32_t out_d = 0;														
	uint32_t out_d_LED = 0;
	uint16_t sampl_count = 0;
	uint32_t last_d = 1;
	extern uint32_t adc_to_led[2];
	
	
	out_d = calculate_val(adc_to_led[chanel]); 			// calc real val
	out_d_LED += out_d;															// sampling(adding) real values
	sampl_count++;
	if (sampl_count == 100){
		out_d_LED /= 100;															// get avarage value of last 100 measuring
		last_d = out_d_LED;														// refresh digit on LED disp
		sampl_count = 0;
	}
	return last_d;
	}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
uint32_t calculate_val_ac(uint32_t adc_val){			// convertion val from adc to real val
	uint32_t val = 0;
	val = adc_val/5000;															// deviding on half of number of sample values because measured only one (positive)half-wave
	return sqrt((double)val);												// return RMS value of measured U
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void store_val(void){
	static uint8_t i_devider = 12;
	static uint8_t u_devider = 165;
	I_accum += ( (adc_val[_I_]*10/i_devider) * (adc_val[_I_]*10/i_devider) );			// devider selected manual 
	U_accum += ( (adc_val[_U_]*100/u_devider) * (adc_val[_U_]*100/u_devider) );		// to have more accuracy & not to work with float val's - "*100"
																																							// & selecteble devider 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void get_real_val(void){
		I_val = calculate_val_ac(I_accum);
		U_val = calculate_val_ac(U_accum);
		I_accum = 0;
		U_accum = 0;
		cnt = 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

