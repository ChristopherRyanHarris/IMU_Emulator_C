
/******************************************************************
** FILE: GaPA_Config.h
** Header for the Gait Phase Angle estimator (GaPA)
** This file should contain only definitions specific to the
** GaPA algorithms
******************************************************************/

#ifndef GAPA_H
#define GAPA_H


/*******************************************************************
** Defines
********************************************************************/

#define GAPA_ON 1

/*******************************************************************
** Tyedefs
********************************************************************/

/* 
** TYPE: GAPA_STATE_TYPE
** This holds the state variables
** for the full Gait Phase Angle estimation code.
*/
typedef struct
{
	float phi; /* "thigh angle wrt vertical down (i.e. Pitch) */ 
	float phi_max, phi_min; /* max/min pitch in previous gait */
	float phi_max_next, phi_min_next; /* max/min pitch in this gait (for use in next cycle) */
	float PHI; /* Time integral of the thigh angle (i.e. Integral of the Pitch) */ 
	float PHI_max, PHI_min; /* max/min pitch integral in previous gait */ 
	
	float gamma; /* "right shift variable" */
	float GAMMA; /* "left shift variable" */
	float z; /* the "scale factor" */
	
	float nu;	/* The Phase Angle */
	float nu_prev;	/* The previous Phase Angle */
} GAPA_STATE_TYPE;


#endif // GAPA_H

