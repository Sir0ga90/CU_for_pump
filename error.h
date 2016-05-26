#ifndef _ERROR_H_
#define _ERROR_H_

#include "dip_sw.h"
#include "blink.h"

enum error {
	E_OFF,							// no error
	E_U = 555,					// 
	E_I = 666,
	ELI = 777,
	ELO = 888,
	ELL = 999
};

//----------------------------------------------------------------------------------
inline void check_u(void);
inline void check_i(void);
inline void check_well(void);
inline void check_lvls(void);
inline void error_check(void);

#endif
