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
	g_gapa_state.phi_max = 0.0f; 
	g_gapa_state.phi_min = 0.0f;
	g_gapa_state.PHI_max = 0.0f;
	g_gapa_state.PHI_min = 0.0f;
	g_gapa_state.gamma   = 0.0f;
	g_gapa_state.z       = 0.0f;
	g_gapa_state.nu      = 0.0f;	
}/* End GaPA_Init */


/*****************************************************************
** Function: GaPA_Reset
** The GaPA state variables must be reset 
*/
void GaPA_Reset( void )
{
	g_gapa_state.phi_max = 0.0f;
	g_gapa_state.phi_min = 0.0f;
	g_gapa_state.PHI_max = 0.0f;
	g_gapa_state.PHI_min = 0.0f;
	g_gapa_state.gamma   = 0.0f;
	g_gapa_state.z       = 0.0f;
	g_gapa_state.nu      = 0.0f;	
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
	/* Add check, 
	** If (nu crosses 0) 
	**  Update phi_min, phi_max, PHI_min, PHI_max
	**    Given as, 
	**    phi_min(t) = min{phi(t_hat) | t_hat E [t_phimax,t)}
	**    phi_min(t) = min{phi(t_hat) | t_hat E [t_phimax,t)}
	**    phi_min(t) = min{phi(t_hat) | t_hat E [t_phimax,t)}
	**    phi_min(t) = min{phi(t_hat) | t_hat E [t_phimax,t)}
	**	Reset GaPA state variables
	**
	*/
	
	
	
	calc_SftPrmLeft();
	calc_SftPrmRight();
	calc_ScaleFactor();
	calc_PhaseAngle();
	
}/* End GaPA_Update */


/*****************************************************************
** Function: calc_SftPrmLeft
** Calculate the left side shift parameter "GAMMA"
** where, 
**   GAMMA = -( (PHI_max+PHI_min)/2 )
*/
void calc_SftPrmLeft( void )
{
	
}/* End calc_SftPrmLeft */


/*****************************************************************
** Function: calc_SftPrmRight
** Calculate the right side shift parameter "gamma"
** where, 
**   gamma = -( (phi_max+phi_min)/2 )
*/
void calc_SftPrmRight( void )
{
	
}/* End calc_SftPrmRight */


/*****************************************************************
** Function: calc_ScaleFactor
** Calculate the shift parameter "z"
** where, 
**   z = abs(phi_max-phi_min)/abs(PHI_max-PHI_min)
*/
void calc_ScaleFactor( void )
{
	
}/* End calc_ScaleFactor */


/*****************************************************************
** Function: calc_PhaseAngle
** Calculate the phase angle "nu"
** where, 
**   nu = atan2( -z*(PHI+GAMMA), -(phi+gamma) )
*/
void calc_PhaseAngle( void )
{
	
}/* End calc_PhaseAngle */



