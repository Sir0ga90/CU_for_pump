/**
	@author Sir0ga90
	@version 1.0
*/
#include "blink.h"
#include "stm32f0xx_hal.h"
#include "level.h"
#include "my_func.h"
#include "error.h"
//--------------------------------------------------------------------------
void delay(){
	HAL_Delay(6);
}
//--------------------------------------------------------------------------
void dig_to_port(uint8_t dig, uint8_t dp){
	init_dig();
	if (dp){	 // if digit with decimal point
		on_dig(dig);
		HAL_GPIO_WritePin(IND_PRT_A, DP, GPIO_PIN_SET);
	}
	else 		on_dig(dig);				// if only digit
}

//--------------------------------------------------------------------------
void on_seg_1(void){
	HAL_GPIO_WritePin(IND_PRT_B, C1, GPIO_PIN_SET);
}
//--------------------------------------------------------------------------
void on_seg_2(void){
	HAL_GPIO_WritePin(IND_PRT_B, C2, GPIO_PIN_SET);
}
//--------------------------------------------------------------------------
void on_seg_3(void){
	HAL_GPIO_WritePin(IND_PRT_B, C3, GPIO_PIN_SET);
}
//--------------------------------------------------------------------------
void off_seg_1(void){
	HAL_GPIO_WritePin(IND_PRT_B, C1, GPIO_PIN_RESET);
}
//--------------------------------------------------------------------------
void off_seg_2(void){
	HAL_GPIO_WritePin(IND_PRT_B, C2, GPIO_PIN_RESET);
}
//--------------------------------------------------------------------------
void off_seg_3(void){
	HAL_GPIO_WritePin(IND_PRT_B, C3, GPIO_PIN_RESET);
}
//--------------------------------------------------------------------------


void on_dig(uint8_t digit){
	switch (digit){
		case 0:
			HAL_GPIO_WritePin(IND_PRT_A, F | B | C, GPIO_PIN_SET);
			HAL_GPIO_WritePin(IND_PRT_B, A | D | E, GPIO_PIN_SET);
			break;
		case 1:
			HAL_GPIO_WritePin(IND_PRT_A, B | C, GPIO_PIN_SET);
			break;
		case 2:
			HAL_GPIO_WritePin(IND_PRT_A, B | G, GPIO_PIN_SET);
			HAL_GPIO_WritePin(IND_PRT_B, A | E | D, GPIO_PIN_SET);
			break;
		case 3:
			HAL_GPIO_WritePin(IND_PRT_A, B | C | G, GPIO_PIN_SET);
			HAL_GPIO_WritePin(IND_PRT_B, A | D, GPIO_PIN_SET);
			break;
		case 4:
			HAL_GPIO_WritePin(IND_PRT_A, B | C | F | G, GPIO_PIN_SET);
			break;
		case 5:
			HAL_GPIO_WritePin(IND_PRT_A, C | F | G, GPIO_PIN_SET);
			HAL_GPIO_WritePin(IND_PRT_B, A | D, GPIO_PIN_SET);
			break;
		case 6:
			HAL_GPIO_WritePin(IND_PRT_A, C | F | G, GPIO_PIN_SET);
			HAL_GPIO_WritePin(IND_PRT_B, A | D | E, GPIO_PIN_SET);
			break;
		case 7:
			HAL_GPIO_WritePin(IND_PRT_A, B | C, GPIO_PIN_SET);
			HAL_GPIO_WritePin(IND_PRT_B, A, GPIO_PIN_SET);
			break;
		case 8:
			HAL_GPIO_WritePin(IND_PRT_A, B | C | F | G, GPIO_PIN_SET);
			HAL_GPIO_WritePin(IND_PRT_B, A | D | E, GPIO_PIN_SET);
			break;
		case 9:
			HAL_GPIO_WritePin(IND_PRT_A, B | C | F | G, GPIO_PIN_SET);
			HAL_GPIO_WritePin(IND_PRT_B, A | D, GPIO_PIN_SET);
			break;
	}
}
//--------------------------------------------------------------------------
void init_dig(void){
	HAL_GPIO_WritePin(IND_PRT_A, B | C | F | G, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IND_PRT_B, A | D | E, GPIO_PIN_RESET);
}
//--------------------------------------------------------------------------
void init_disp(void){
	off_seg_1();
	off_seg_2();
	off_seg_3();
	HAL_GPIO_WritePin(IND_PRT_A, DP, GPIO_PIN_RESET);
}
//--------------------------------------------------------------------------
uint32_t disp_chanel(void){
	if (chanel == _I_)
		return I_val;   // * 10 decimal part of current
	else 
		return U_val;
}
//---------------------------------------------------------------------------
uint32_t disp_err_chanel(void){
	if (error_type == E_I || error_type == ELI)
		return last_val_before_err;   // * 10 decimal part of current
	else 
		return U_val;
}
//--------------------------------------------------------------------------
uint16_t disp_symbols_pA[] = {
									// led_seg on port_A:---------> B(bit11), C(10), F(9), G(12), DP(8)

	0xE00,		//0
	0xC00,		//1
	0x1800,		//2
	0x1C00,		//3
	0x1E00, 	//4
	0x1600, 	//5
	0x1600,		//6
	0x0C00,		//7
	0x1E00,		//8
	0x1E00,		//9
	0x1200,		//'E'
	0x0200,		//'L'
	0x0E00,		//'U'
	0x0200,		//'I'
	0x1200,		//'F'
	0x1000,		//'r'
	0x0000,		//'_'
	0x1000		//'-'
};
//-------------------------------------------------
uint16_t disp_symbols_pB[] = {
									// led_seg on port_B:---------> A(bit15), D(14), E(13)
	
	0xE000,		//0
	0x0000, 	//1
	0xE000,		//2 
	0xC000, 	//3
	0x0000, 	//4
	0xC000,		//5 
	0xE000, 	//6
	0x8000, 	//7
	0xE000, 	//8
	0xC000,		//9
	0xE000,   //'E'
	0x6000,		//'L'
	0x6000,		//'U'
	0x2000,		//'I'
	0xA000,		//'F'
	0x2000,		//'r'
	0x4000,		//'_'
	0x0000		//'-'
};


//--------------------------------------------------------------------------
void dig_to_disp_a(Disp_sym *out_dig, Val_on_disp *odl_f){
	static uint8_t counter  = 0;
	static uint8_t dp = 0;
	
	switch (counter){
		
		case 0:
			init_disp();
			on_seg_1();
			if (*odl_f == e_r_timer) dp = 1;
			dig_to_port_a(out_dig[counter], dp);
			dp = 0;
			break;
		case 1:
			init_disp();
			on_seg_2();
			
			if ( (error_type == E_I || error_type == ELI || chanel == _I_) && *odl_f == e_cnl){
				dp = 1;
			}
				
			dig_to_port_a(out_dig[counter], dp);															
			break;
		case 2:
			init_disp();
			on_seg_3();
			dig_to_port_a(out_dig[counter], dp);
			break;
	}
	dp = 0;
	counter++;
	if (counter > 2) counter = 0;
}
//---------------------------------------------
void on_dig_a(Disp_sym sym){
			GPIOA->ODR |= disp_symbols_pA[sym];
			GPIOB->ODR |= disp_symbols_pB[sym];
}
//-------------------------------------------------
void dig_to_port_a(Disp_sym dig, uint8_t dp){
	init_dig();
	if (dp){	 // if digit with decimal point
		on_dig(dig);
		HAL_GPIO_WritePin(IND_PRT_A, DP, GPIO_PIN_SET);
	}
	else 		on_dig_a(dig);				// if only digit
}
//---------------------------------------------
void int_to_sym(uint32_t sym, Disp_sym* sym_a){
	uint8_t cnt = 3;
	uint8_t i, j;
	
	for (i = 0, j = 2; i<cnt; i++, j--){
		sym_a[j] = sym%10;
		sym /= 10;
	}
	
}
