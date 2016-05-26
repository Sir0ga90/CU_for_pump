#ifndef LOGIC_H
#define LOGIC_H

#include "stm32f0xx_hal.h"
#include "dip_sw.h"
#include "level.h"

inline void pump(void);
inline void drain(void);
inline void work_logic(void);

typedef enum{
	e_err, e_pump, e_drain
} Logic;

#endif
