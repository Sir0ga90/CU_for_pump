#include "my_func.h"
#include "blink.h"

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint32_t calculate_val(uint32_t adc_val){														// convertion val from adc to real val
	
	static const uint16_t devide_coef = 1220;																// coefficient for convertion, practical val (calculate val - 4095/330 = 12,41)
	return (adc_val*100)/devide_coef;																	// F.E. for 12 bit adc for 4095 = 3.3V: 4095*100/1220, 
																																		// * on 100 for flash val in 3 seg disp
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8_t counter = 0;		// counter for sigments of led: for 1 interrupt 1 segment
uint32_t tmp = 0;
uint32_t dig = 0;
uint8_t dp = 0;
void dig_to_disp(uint32_t out_dig){
	switch (counter){
		case 0:
			tmp = out_dig;
			dig = tmp%10;			
			tmp = tmp/10;
			init_disp();
			on_seg_1();
			dig_to_port(dig, dp);
			break;
		case 1:
			dig = tmp%10;
			tmp = tmp/10;
			init_disp();
			on_seg_2();
			dig_to_port(dig, dp);
			break;
		case 2:
			dig = tmp%10;
			init_disp();
			on_seg_3();
			//dp = 1;
			dig_to_port(tmp, dp);
			break;
	}
	dp = 0;
	counter++;
	if (counter > 2) counter = 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
	uint8_t i = 0;
	uint32_t disp_val = 0;
	uint32_t last_val = 0;

uint32_t sample_val_2(uint32_t new_adc){
	disp_val += new_adc;
	
	if (i > 99){
		disp_val /= 100;
		last_val = disp_val;
	} 
	i++;
	return last_val;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void read_button(void){
	HAL_Delay(50);
	switch(button){
		case 1:{
			if (button == 1) chanel++;
			if (chanel == toggle) chanel = _I_;
				button = 0;
			break;
			}
		case 3:{
			HAL_GPIO_WritePin(work_led_GPIO_Port, work_led_Pin, GPIO_PIN_SET);
			button = 0;
			break;
		}
		default:{
			button = 0;
			break;
		}
			
	}
	
			
}
