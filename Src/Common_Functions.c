
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

  p_control->timestamp      = 0;
  p_control->timestamp_old  = 0;
  p_control->G_Dt           = 0.0;

	/* An emu timestamp is needed  */
	#if EXE_MODE==1 /* Emulator Mode */
		p_control->emu_data.timestamp = 0;

		p_control->emu_data.InputFID  = NULL;
		p_control->emu_data.OutputFID = NULL;
	#endif

  /* If in calibration mode,
	** set default calibration parameters */
	#if CALIBRATION_MODE==1
  	p_control->calibration_prms.output_mode = CAL_OUTPUT_MODE;
  #endif

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

