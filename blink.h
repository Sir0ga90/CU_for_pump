// by Sir0ga, 22.03.2016


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




#define A 	GPIO_PIN_13
#define B 	GPIO_PIN_9
#define C 	GPIO_PIN_10
#define D 	GPIO_PIN_14
#define E 	GPIO_PIN_12
#define F 	GPIO_PIN_15
#define G 	GPIO_PIN_11
#define DP 	GPIO_PIN_8


#define C1		GPIO_PIN_6			//cathod of digit 1 (from right)
#define C2		GPIO_PIN_12			//cathod of digit 2 (from right)
#define C3		GPIO_PIN_13			//cathod of digit 3 (from right)

//#define D_0		(A | B | C | D | E | F)
//#define D_1		(B | C)
//#define D_2		(A | B | G | E | D)
//#define D_3 	(A | B | C | G | D)
//#define D_4		(F | G | B | C)
//#define D_5 	(A | F | G | C | D)
//#define D_6 	(A | F | G | C | D | E)
//#define D_7 	(A | B | C)
//#define D_8		(A | B | C | D | E | F | G)
//#define D_9 	(A | B | C | D | F | G)


#define IND_PRT_A 		GPIOA
#define IND_PRT_B 		GPIOB
#define IND_PRT_F 		GPIOF

