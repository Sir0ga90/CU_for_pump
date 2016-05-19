// by Sir0ga, 22.03.2016
#ifndef BLINK_H
#define BLINK_H

#include "stm32f030x8.h"
void delay(void);
void dig_to_port(uint8_t dig, uint8_t dp);

void on_seg_1(void);
void on_seg_2(void);
void on_seg_3(void);

void off_seg_1(void);
void off_seg_1(void);
void off_seg_1(void);

void init_dig(void);
void on_dig(uint8_t digit);
void init_disp(void);

uint32_t disp_chanel(void);


#define A 	GPIO_PIN_15
#define B 	GPIO_PIN_11
#define C 	GPIO_PIN_10
#define D 	GPIO_PIN_14
#define E 	GPIO_PIN_13
#define F 	GPIO_PIN_9
#define G 	GPIO_PIN_12
#define DP 	GPIO_PIN_8


#define C1		GPIO_PIN_11			//cathod of digit 1 (from right)
#define C2		GPIO_PIN_10			//cathod of digit 2 (from right)
#define C3		GPIO_PIN_12			//cathod of digit 3 (from right)


#define IND_PRT_A 		GPIOA
#define IND_PRT_B 		GPIOB

#endif
