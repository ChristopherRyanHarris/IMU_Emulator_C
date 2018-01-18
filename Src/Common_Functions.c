
/*******************************************************************
** FILE: Common_Functions
** This file contains several setup and initialization functions
** which are common accross all execution platforms.
*/


/*******************************************************************
** Includes ********************************************************
********************************************************************/


#include "../Include/Common_Config.h"
#if EXE_MODE==1 /* Emulator Mode */
	#include "../Include/Emulator_Config.h"
	#include "../Include/Emulator_Protos.h"

	#ifdef _IMU10736_
		#include "../Include/IMU10736_Config.h"
	#endif
	#ifdef _IMU9250_
		#include "../Include/IMU9250_Config.h"
	#endif

#endif  /* End Emulator Mode */


/*******************************************************************
** Functions *******************************************************
********************************************************************/


/*************************************************
** FUNCTION: Common_Init
** VARIABLES:
**		[IO]	CONTROL_TYPE	*p_control
** RETURN:
**		NONE
** DESCRIPTION:
** 		This function initializes variables and constants which
** 		are the same across all platforms and which are common
** 		across all agorithm variants
*/
void Common_Init ( CONTROL_TYPE *p_control )
{
  LOG_PRINT("> Initializing Common\n");

	/* Set default IO mode */
	p_control->output_mode = OUTPUT_MODE;

	/* Set common exe parameters */
  p_control->timestamp      = 0;
  p_control->timestamp_old  = 0;
  p_control->G_Dt           = 0.0;

	/* For emulation mode, 
	** am "emu timestamp" is needed  */
	#if EXE_MODE==1 /* Emulator Mode */
		p_control->emu_data.timestamp = 0;
		p_control->emu_data.InputFID  = NULL;
		p_control->emu_data.OutputFID = NULL;
	#endif


	p_control->verbose        = DEBUG; 
	p_control->calibration_on = CALIBRATION_MODE;
	p_control->DSP_on         = DSP_ON;
	p_control->DCM_on					= DCM_ON;
	p_control->GaPA_on        = GAPA_ON;
	p_control->WISE_on        = WISE_ON;
	
	/* Set mode parameters */
	p_control->sensor_prms.gravity     = GRAVITY;
	p_control->sensor_prms.accel_on    = ACCEL_ON;
	p_control->sensor_prms.gyro_on     = GYRO_ON;
	p_control->sensor_prms.magn_on     = MAGN_ON;
	p_control->sensor_prms.sample_rate = TIME_SR;
	
	
	/* DSP Init */
	p_control->dsp_prms.n_taps = NTAPS;
	p_control->dsp_prms.IIR_on = FIR_on;
	p_control->dsp_prms.FIR_on = IIR_on;
	
	p_control->dcm_prms.Kp_RollPitch = Kp_ROLLPITCH;
	p_control->dcm_prms.Ki_RollPitch = Ki_ROLLPITCH;
	p_control->dcm_prms.Kp_Yaw       = Kp_YAW;
	p_control->dcm_prms.Ki_Yaw       = Ki_YAW;
	
	p_control->dcm_prms.PitchOrientation  = PITCH_O;
	p_control->dcm_prms.PitchRotationConv = PITCH_ROT_CONV;
	p_control->dcm_prms.RollRotationConv  = ROLL_ROT_CONV;
	p_control->dcm_prms.PitchRotationConv = ROLL_ZREF;
	

  /* If in calibration mode,
	** set default calibration parameters */
	if( p_control->cal_mode==1 )
  {
  	p_control->calibration_prms.output_mode = CAL_OUTPUT_MODE;
  	
  	p_control->calibration_prms.accel_min_x = ACCEL_X_MIN;
  	p_control->calibration_prms.accel_max_x = ACCEL_X_MAX;
  	p_control->calibration_prms.accel_min_y = ACCEL_Y_MIN;
  	p_control->calibration_prms.accel_max_y = ACCEL_Y_MAX;
  	p_control->calibration_prms.accel_min_z = ACCEL_Z_MIN;
  	p_control->calibration_prms.accel_max_z = ACCEL_Z_MAX;
  	
  	p_control->calibration_prms.gyro_ave_offset_x = GYRO_AVERAGE_OFFSET_X;
  	p_control->calibration_prms.gyro_ave_offset_y = GYRO_AVERAGE_OFFSET_Y;
  	p_control->calibration_prms.gyro_ave_offset_z = GYRO_AVERAGE_OFFSET_Z;
  	
  	p_control->calibration_prms.magn_min_x = MAGN_X_MIN;
  	p_control->calibration_prms.magn_max_x = MAGN_X_MAX;
  	p_control->calibration_prms.magn_min_y = MAGN_Y_MIN;
  	p_control->calibration_prms.magn_max_y = MAGN_Y_MAX;
  	p_control->calibration_prms.magn_min_z = MAGN_Z_MIN;
  	p_control->calibration_prms.magn_max_z = MAGN_Z_MAX;
  }

} /* End Common_Init*/


/*************************************************
** FUNCTION: UpdateTime
** VARIABLES:
**		[IO]	CONTROL_TYPE	*p_control
** RETURN:
**		NONE
** DESCRIPTION:
** 		Update the time state
** 		Delta time (s) is used to determine the state
** 		estimate in the filter.
*/
void Update_Time( CONTROL_TYPE *p_control )
{
  #if EXE_MODE==1 /* Emulator Mode */
  	/* Timestamp is read from file */
  	p_control->timestamp_old = p_control->timestamp;
  	p_control->timestamp     = p_control->emu_data.timestamp;

  #else /* Real Time mode */
  	float minTime = (float) (TIME_RESOLUTION / (TIME_SR+1.0) ); /* Set Sampling Rate */
  	while( (TIME_FUPDATE - p_control->timestamp) < (minTime) ) {}
  	/* Update delta T */
  	p_control->timestamp_old = p_control->timestamp;
  	p_control->timestamp     = TIME_FUPDATE;

  #endif /* End Emulator Mode */

	/* Get delta t */
  if( p_control->timestamp_old > 0 )
	{
		p_control->G_Dt = (float) ( (p_control->timestamp - p_control->timestamp_old) / TIME_RESOLUTION ) ;
	}
  else
  {
  	p_control->G_Dt = 0.0f;
  }
} /* End Update_Time */

