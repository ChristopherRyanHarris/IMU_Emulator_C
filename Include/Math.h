
/*******************************************************************
** FILE: 
**   	Math.h
** DESCRIPTION: 
** 		This header file is inteded to hold common mathmatical
**		macros and commonly used constants.
********************************************************************/


#ifndef MATH_H
#define MATH_H

#include "Common_Config.h"

#define FALSE 0
#define TRUE 1

#define GTOMPS2 (9.80665)
#define MPSTOMPH (2.23694)

#define PI (3.14159265359)

#if EXE_MODE==1 /* Emulator mode */
	#define FCONSTRAIN(x,m,M) (fmin(fmax(x,m),M))
#else
	#define FCONSTRAIN constrain
#endif /* EXE_MODE */

#define SIGN(x) ( (0<x)-(x<0)+(x==0) )

#define MAX( a, b ) ( ( a > b) ? a : b )
#define MIN( a, b ) ( ( a < b) ? a : b )

#endif /* End MATH_H */
