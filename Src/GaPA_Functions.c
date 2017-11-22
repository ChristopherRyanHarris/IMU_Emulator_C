/*************************************************
** FILE: GaPa_Functions
** This file contains all functions spacific to the
** Gait Phase Angle estimation (GaPA) capabilities
**************************************************/


/*******************************************************************
** Includes ********************************************************
********************************************************************/

#include "../Include/Common_Config.h"

#if EXE_MODE /* Emulator mode */
#include <math.h>
#include <string.h>

#ifdef _IMU10736_
#include "../Include/IMU10736_Config.h"
#endif
#ifdef _IMU9250_
#include <SparkFunMPU9250-DMP.h>
#include "../Include/IMU9250_Config.h"
#endif

#include "../Include/Math.h"
#include "../Include/GaPA_Config.h"
#include "../Include/DCM_Config.h"
#include "../Include/Emulator_Config.h"

extern CAL_STATE_TYPE      g_calibration;
extern DCM_STATE_TYPE      g_dcm_state;
extern DSP_COMMON_TYPE     g_dsp;
extern SENSOR_STATE_TYPE   g_sensor_state;
extern CONTROL_STATE_TYPE  g_control_state;
extern GAPA_STATE_TYPE     g_gapa_state;
#endif /* End Emulator mode */

/*******************************************************************
** Functions *******************************************************
********************************************************************/




/*****************************************************************
** Function: GaPA_Init
** This function initializes the GaPA state
** variables.
*/
void GaPA_Init( void )
{
	/* The version of the phase portrait to use
	**  1:PHI
	**  2:PHV
	** NOTE: Only PHI implemented at this time */
	g_gapa_state.version      = 1;
	
	g_gapa_state.phi_max      = 0.0f; 
	g_gapa_state.phi_min      = 0.0f;
	g_gapa_state.PHI_max      = 0.0f;
	g_gapa_state.PHI_min      = 0.0f;
	g_gapa_state.phi_max_next = 0.0f;
	g_gapa_state.phi_min_next = 0.0f;
	g_gapa_state.PHI_max_next = 0.0f;
	g_gapa_state.PHI_min_next = 0.0f;
	g_gapa_state.gamma        = 0.0f;
	g_gapa_state.GAMMA        = 0.0f;
	g_gapa_state.z            = 0.0f;
	g_gapa_state.nu           = 0.0f;	
}/* End GaPA_Init */


/*****************************************************************
** Function: GaPA_Reset
** The GaPA state variables must be reset 
*/
void GaPA_Reset( void )
{
	g_gapa_state.phi_max_next = 0.0f;
	g_gapa_state.phi_min_next = 0.0f;
	g_gapa_state.PHI_max_next = 0.0f;
	g_gapa_state.PHI_min_next = 0.0f;
	g_gapa_state.gamma        = 0.0f;
	g_gapa_state.GAMMA        = 0.0f;
	g_gapa_state.z            = 0.0f;
}/* End GaPA_Reset */



/*****************************************************************
** Function: GaPA_Update
** This function runs the Gait Phase Angle (GaPA) estimator
** This code will execute the code necessary to 
** solve for the phasae variable "nu" 
** where, 
**   nu = atan2( -z*(PHI+GAMMA), -(phi+gamma) )
*/
void GaPA_Update( void )
{
	
	/* Store previous nu 
	** NOTE: On the first cycle, this is meaningless since 
	**       both nu and nu_prev are zero */
	g_gapa_state.nu_prev = g_gapa_state.nu;
	
	
	/* Set our phase variables phi and PHI
	** NOTE: version [1,2]:
	** 	1: PHI
	**		 phi: The thigh angle (pitch)
	**		 PHI: The integral of the thigh angle (swing distance)
	**  2: PHV - NOT IMPLEMENTED!
	**		 phi: The thigh angular velocity (swing angular velocity)
	**		 PHI: The thigh angle (pitch)
	*/
	switch( g_gapa_state.version )
	{
		case 1 : /* PHI */
			g_gapa_state.phi =  g_sensor_state.pitch;
			g_gapa_state.PHI += g_sensor_state.pitch*g_control_state.G_Dt;
			break;
		case 2 : /* PHV */
			//g_gapa_state.phi = (g_sensor_state.pitch - g_sensor_state.prev_pitch)*g_control_state.G_Dt;
			//g_gapa.state.PHI = g_sensor_state.pitch;
			break;
		default :
			// Need to add some catch statements
			break;
	} /* End version switch */
	
	
	/* Track phi min/max and PHI min/max
	** These variables will be used in the next gait cycle to 
	** scale the phase portrait to a constant radius within 
	** each quadrandt to provide an approximately cirsular orbit. */
	TrackPhiVariables( &g_gapa_state );
	
	/* Calculate the new GAMMA variable
	** GAMMA = -( (PHI_max+PHI_min)/2 ) */
	calc_SftPrmLeft( &g_gapa_state.GAMMA, g_gapa_state.PHI_max, g_gapa_state.PHI_min ); 
	
	/* calculate the new gamma variable 
	** gamma = -( (phi_max+phi_min)/2 ) */
	calc_SftPrmRight( &g_gapa_state.gamma, g_gapa_state.phi_max, g_gapa_state.phi_min ); 
	
	/* Calculate the new scaling factor "z"
	** z = abs(phi_max-phi_min)/abs(PHI_max-PHI_min) */
	calc_ScaleFactor( &g_gapa_state.z, 
										 g_gapa_state.phi_max, g_gapa_state.phi_min, 
										 g_gapa_state.PHI_max, g_gapa_state.PHI_min ); 
	
	/* Calculate nu
	**   The phase angle is the angle between the two 
	**   Phase variables phi and PHI.
	** nu = atan2( -z*(PHI+GAMMA), -(phi+gamma) ) */
	calc_PhaseAngle( &g_gapa_state.nu,  g_gapa_state.z, 
										g_gapa_state.PHI, g_gapa_state.GAMMA, 
										g_gapa_state.phi, g_gapa_state.gamma  );  
	
	
	/* The min/max values of the filtered angle and integral are 
	** stored for computing the shift and scale. At the beginning 
	** of each gait cycle (indicated by the phase variable crossing
	** zero) we update the min/max values using the detected min/max
	** from the previous cycle.
	**
	** If (nu crosses 0) { Update phi_min, phi_max, PHI_min, PHI_max  from the previous cycle }
	**    Given as, 
	**    PHI_min(t) = min{ PHI(t_hat) | t_hat ∈ [t_phiM,t) }
	**    phi_min(t) = min{ phi(t_hat) | t_hat ∈ [t_phiM,t) }
	**    PHI_max(t) = max{ PHI(t_hat) | t_hat ∈ [t_phim,t) }
	**    phi_max(t) = max{ phi(t_hat) | t_hat ∈ [t_phim,t) }
	**	Reset GaPA state variables */
	if( fabs(g_gapa_state.nu-g_gapa_state.nu_prev)>PI )
	{
		/* Update min/max values */
		g_gapa_state.phi_min = g_gapa_state.phi_min_next;
		g_gapa_state.phi_max = g_gapa_state.phi_max_next;
		g_gapa_state.PHI_min = g_gapa_state.PHI_min_next;
		g_gapa_state.PHI_max = g_gapa_state.PHI_max_next;
		
		/* Reset our states at the beginning of each new gait cycle s*/
		GaPA_Reset();
	}
}/* End GaPA_Update */

/*****************************************************************
** Function: TrackPhiVariables
** Keep track of the min/max of the phase variables phi and PHI. 
** These will be used to scale the phase portrait to force it to
** Be close to a constant radius. 
*/
void TrackPhiVariables( GAPA_STATE_TYPE* l_gapa_state )
{
	/* Update phi min/max */
	if( l_gapa_state->phi > l_gapa_state->phi_max_next )
	{
		l_gapa_state->phi_max_next = l_gapa_state->phi;
	}
	else if( l_gapa_state->phi < l_gapa_state->phi_min_next )
	{
		l_gapa_state->phi_min_next = l_gapa_state->phi;
	}
	
	/* Update PHI min/max */
	if( l_gapa_state->PHI > l_gapa_state->PHI_max_next )
	{
		l_gapa_state->PHI_max_next = l_gapa_state->PHI;
	}
	else if( l_gapa_state->PHI < l_gapa_state->PHI_min_next )
	{
		l_gapa_state->PHI_min_next = l_gapa_state->PHI;
	}
} /* End TrackPhiVariables */


/*****************************************************************
** Function: calc_SftPrmLeft
** Calculate the left side shift parameter "GAMMA"
** where, 
**   GAMMA = -( (PHI_max+PHI_min)/2 )
*/
void calc_SftPrmLeft( float* GAMMA, float PHI_max, float PHI_min )
{
	(*GAMMA) = -( (g_gapa_state.PHI_max+PHI_min)*0.5 );
}/* End calc_SftPrmLeft */


/*****************************************************************
** Function: calc_SftPrmRight
** Calculate the right side shift parameter "gamma"
** where, 
**   gamma = -( (phi_max+phi_min)/2 )
*/
void calc_SftPrmRight( float* gamma, float phi_max, float phi_min )
{
	(*gamma) = -( (phi_max+phi_min)*0.5 );
}/* End calc_SftPrmRight */


/*****************************************************************
** Function: calc_ScaleFactor
** Calculate the shift parameter "z"
** where, 
**   z = abs(phi_max-phi_min)/abs(PHI_max-PHI_min)
*/
void calc_ScaleFactor( float *z, float phi_max, float phi_min, float PHI_max, float PHI_min )
{
		g_gapa_state.z = fabs( (g_gapa_state.phi_max-phi_min)/(g_gapa_state.PHI_max-PHI_min) );
}/* End calc_ScaleFactor */


/*****************************************************************
** Function: calc_PhaseAngle
** Calculate the phase angle "nu"
** where, 
**   nu = atan2( -z*(PHI+GAMMA), -(phi+gamma) )
*/
void calc_PhaseAngle( float* nu, float z, float PHI, float GAMMA, float phi, float gamma )
{
	(*nu) = f_atan2( (-z*(PHI+GAMMA)) , (-phi+gamma) );
}/* End calc_PhaseAngle */



