
/*************************************************
** FILE: CAL_Functions
** This file contains the functions which will help
** calibration the sensors
*/


/*******************************************************************
** Includes ********************************************************
********************************************************************/


#ifndef EXE_MODE==1
	#include "../Include/Common_Config.h"
#endif
#if EXE_MODE==1 /* Emulator Mode */
	#include "../Include/Emulator_Config.h"
#endif /* End Emulator Mode */


/*******************************************************************
** Functions *******************************************************
********************************************************************/

/*************************************************
** FUNCTION: Calibration_Init
** VARIABLES:
**		[I ]	CONTROL_TYPE			*p_control
**		[IO]	CALIBRATION_TYPE	*p_calibration
** RETURN:
**		NONE
** DESCRIPTION:
** 		This function initializes the calibration state
** 		variables.
*/
void Calibration_Init ( CONTROL_TYPE			*p_control,
												CALIBRATION_TYPE 	*p_calibration )
{
  int i;

  LOG_PRINTLN("> Initializing Calibration");

  for( i=0; i<3; i++ )
  {
    p_calibration->accel_total[i] = 0.0f;
    p_calibration->accel_max[i]   = -9999.0f;
    p_calibration->accel_min[i]   = 9999.0f;

    p_calibration->gyro_total[i]  = 0.0f;
    p_calibration->gyro_max[i]    = -9999.0f;
    p_calibration->gyro_min[i]    = 9999.0f;
  }
  p_calibration->N = 0;
} /* End Calibration_Init */


/*************************************************
** FUNCTION: Calibrate
** VARIABLES:
**		[I ]	CONTROL_TYPE			*p_control
**		[IO]	CALIBRATION_TYPE	*p_calibration
**		[I ]	SENSOR_STATE_TYPE	*p_sensor_state
** RETURN:
**		NONE
** DESCRIPTION:
** 		This function initializes the calibration state
** 		variables.
*/
void Calibrate ( CONTROL_TYPE				*p_control,
								 CALIBRATION_TYPE		*p_calibration,
								 SENSOR_STATE_TYPE	*p_sensor_state )
{
  int i;
  for( i=0; i<3; i++ )
  {
    p_calibration->accel_total[i] += p_sensor_state->accel[i];
    if( p_sensor_state->accel[i] > p_calibration->accel_max[i] ) { p_calibration->accel_max[i] = p_sensor_state->accel[i]; }
    if( p_sensor_state->accel[i] < p_calibration->accel_min[i] ) { p_calibration->accel_min[i] = p_sensor_state->accel[i]; }

    p_calibration->gyro_total[i] += p_sensor_state->gyro[i];
    if( p_sensor_state->gyro[i] > p_calibration->gyro_max[i] ) { p_calibration->gyro_max[i] = p_sensor_state->gyro[i]; }
    if( p_sensor_state->gyro[i] < p_calibration->gyro_min[i] ) { p_calibration->gyro_min[i] = p_sensor_state->gyro[i]; }
  }
  p_calibration->N++;

} /* End Calibrate */









