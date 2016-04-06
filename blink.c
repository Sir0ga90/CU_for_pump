#include "Blink.h"
#include "stm32f0xx_hal.h"

extern uint16_t out_dig;


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
	HAL_GPIO_WritePin(IND_PRT_F, C1, GPIO_PIN_SET);
}
//--------------------------------------------------------------------------
void on_seg_2(void){
	HAL_GPIO_WritePin(IND_PRT_A, C2, GPIO_PIN_SET);
}
//--------------------------------------------------------------------------
void on_seg_3(void){
	HAL_GPIO_WritePin(IND_PRT_A, C3, GPIO_PIN_SET);
}
//--------------------------------------------------------------------------
void off_seg_1(void){
	HAL_GPIO_WritePin(IND_PRT_F, C1, GPIO_PIN_RESET);
}
//--------------------------------------------------------------------------
void off_seg_2(void){
	HAL_GPIO_WritePin(IND_PRT_A, C2, GPIO_PIN_RESET);
}
//--------------------------------------------------------------------------
void off_seg_3(void){
	HAL_GPIO_WritePin(IND_PRT_A, C3, GPIO_PIN_RESET);
}
//--------------------------------------------------------------------------


void on_dig(uint8_t digit){
	switch (digit){
		case 0:
			HAL_GPIO_WritePin(IND_PRT_A, B | C, GPIO_PIN_SET);
			HAL_GPIO_WritePin(IND_PRT_B, A | D | E | F, GPIO_PIN_SET);
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
			HAL_GPIO_WritePin(IND_PRT_A, B | C | G, GPIO_PIN_SET);
			HAL_GPIO_WritePin(IND_PRT_B, F, GPIO_PIN_SET);
			break;
		case 5:
			HAL_GPIO_WritePin(IND_PRT_A, C | G, GPIO_PIN_SET);
			HAL_GPIO_WritePin(IND_PRT_B, A | F | D, GPIO_PIN_SET);
			break;
		case 6:
			HAL_GPIO_WritePin(IND_PRT_A, C | G, GPIO_PIN_SET);
			HAL_GPIO_WritePin(IND_PRT_B, A | D | E | F, GPIO_PIN_SET);
			break;
		case 7:
			HAL_GPIO_WritePin(IND_PRT_A, B | C, GPIO_PIN_SET);
			HAL_GPIO_WritePin(IND_PRT_B, A, GPIO_PIN_SET);
			break;
		case 8:
			HAL_GPIO_WritePin(IND_PRT_A, B | C | G, GPIO_PIN_SET);
			HAL_GPIO_WritePin(IND_PRT_B, A | D | E | F, GPIO_PIN_SET);
			break;
		case 9:
			HAL_GPIO_WritePin(IND_PRT_A, B | C | G, GPIO_PIN_SET);
			HAL_GPIO_WritePin(IND_PRT_B, A | D | F, GPIO_PIN_SET);
			break;
	}
}

//--------------------------------------------------------------------------
void init_dig(void){
	HAL_GPIO_WritePin(IND_PRT_A, B | C | G, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IND_PRT_B, A | D | E | F, GPIO_PIN_RESET);
}
//--------------------------------------------------------------------------
void init_disp(void){
	off_seg_1();
	off_seg_2();
	off_seg_3();
	HAL_GPIO_WritePin(IND_PRT_A, DP, GPIO_PIN_RESET);
}
