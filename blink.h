// by Sir0ga, 22.03.2016
#ifndef BLINK_H
#define BLINK_H

#include "stm32f030x8.h"
#include "error.h"
//--------//--------//--------//--------//--------//--------//--------//------
typedef enum E_disp_symb{
	tE = 10,
	tL,
	tU,
	tI,
	tF,
	tr,
	t_,
	tdash
}Disp_sym;
//--------//--------//--------//--------//--------//--------//--------//------
inline void delay(void);
inline void dig_to_port(uint8_t dig, uint8_t dp);

inline void on_seg_1(void);
inline void on_seg_2(void);
inline void on_seg_3(void);

inline void off_seg_1(void);
inline void off_seg_1(void);
inline void off_seg_1(void);

inline void init_dig(void);
inline void on_dig(uint8_t digit);
inline void init_disp(void);

inline void dig_to_port_a(Disp_sym dig, uint8_t dp);
inline void out_dig_a(Disp_sym sym);
inline void dig_to_disp_a(Disp_sym *out_dig, Val_on_disp *odl_f);
inline void on_dig_a(Disp_sym sym);
inline void int_to_sym(uint32_t sym, Disp_sym* sym_a);
//--------//--------//--------//--------//--------//--------//--------//------
extern uint16_t out_dig;
extern uint32_t I_val;
extern uint32_t U_val;

extern Error error_type;
extern uint32_t last_val_before_err;
//--------//--------//--------//--------//--------//--------//--------//------
inline uint32_t disp_chanel(void);
inline uint32_t disp_err_chanel(void);

inline uint32_t disp_chanel_a(void);

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
