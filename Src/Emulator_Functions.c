
/*******************************************************************
** FILE:
**   	Emulator_Functions
** DESCRIPTION:
** 		This file contains the helper functions exclusive to running
** 		emulation mode. These functions are implemented separatly
** 		(or not at all) in real-time mode.
********************************************************************/


/*******************************************************************
** Includes ********************************************************
********************************************************************/

#ifndef COMMON_CONFIG_H
	#include "../Include/Common_Config.h"
#endif
#if EXE_MODE==1 /* Emulator Mode */
	/* In emulatiom mode, "Emulator_Protos" is needed to
	** use funcitons in other files.
	** NOTE: This header should contain the function
	** 			 prototypes for all execution functions */
	#include "../Include/Emulator_Protos.h"
#endif  /* End Emulator Mode */

/* Only link if in emulation mode */
#if EXE_MODE==1

/*******************************************************************
** Functions *******************************************************
********************************************************************/

/*************************************************
** FUNCTION: Init_Emulator
** VARIABLES:
**		[I ]	CONTROL_TYPE		*p_control
** RETURN:
**		NONE
** DESCRIPTION:
**		This function initialized the control struct to
**		to handle emulation mode execution.
*/
void Init_Emulator( CONTROL_TYPE *p_control )
{
  LOG_PRINT("> Initializing Emulator\n");

  p_control->timestamp      = 0;
  p_control->timestamp_old  = 0;
  p_control->G_Dt           = 0.0;

  p_control->BaudLock       = TRUE;
  p_control->LedState       = FALSE;
  p_control->LastBlinkTime  = 0;
} /* End Init_Emulator */


/*************************************************
** FUNCTION: Read_Sensors
** VARIABLES:
**		[I ]	CONTROL_TYPE			*p_control
**		[IO]	SENSOR_STATE_TYPE	*p_sensor_state
** RETURN:
**		NONE
** DESCRIPTION:
*/
void Read_Sensors( CONTROL_TYPE				*p_control,
								 	 SENSOR_STATE_TYPE	*p_sensor_state )
{
  float temp;
  int   count;

	float gyro_mag;
	float accel_mag;

  count = fread(&temp,sizeof(float),1,p_control->emu_data.InputFID);
  if ( count != 1 )
  {
    fprintf(stdout,"Emulator Exit\n");
    p_control->emu_data.flag = 0;
    return;
  }

  //Format:1
  // Data Collection: 1
  // Format:
  //   14x[float32] 14x[float32] ....
  //   [Timestamp][accel1][accel2][accel3][gyro1][gyro2][gyro3][yaw][pitch][roll][aest_ave1][aest_ave2][vest_ave1][vest_ave2]
  //p_control->emu_data.timestamp = (unsigned long) temp;
  //fread(&p_sensor_state->accel[0],sizeof(float),3,p_control->emu_data.InputFID);
  //fread(&p_sensor_state->gyro[0],sizeof(float),3,p_control->emu_data.InputFID);
  //fseek(p_control->emu_data.InputFID,sizeof(float)*7,SEEK_CUR);


  // Format: 2
  // Data Collection: 2
  //   10x[float32] 10x[float32] ...
  //   [1]        [2]     [3]     [4]     [5]    [6]    [7]    [8]  [9]    [10]
  //   [Timestamp][accel1][accel2][accel3][gyro1][gyro2][gyro3][yaw][pitch][roll]
  p_control->emu_data.timestamp = (unsigned long) temp;
  fread(&p_sensor_state->accel[0],sizeof(float),3,p_control->emu_data.InputFID);
  fread(&p_sensor_state->gyro[0],sizeof(float),3,p_control->emu_data.InputFID);
  fseek(p_control->emu_data.InputFID,sizeof(float)*3,SEEK_CUR);

  /* Increment sample number count */
  p_control->SampleNumber++;

  /* Update gyro stats */
  gyro_mag  = Vector_Magnitude ( &p_sensor_state->gyro[0] );
  if( p_control->SampleNumber == 1 )
  {
  	p_sensor_state->gyro_Ave = gyro_mag;
  	p_sensor_state->gyro_mAve = gyro_mag;
	  p_sensor_state->gyro_M2   = gyro_mag;
	  p_sensor_state->gyro_sVar = 0.0;
	  p_sensor_state->gyro_pVar = 0.0;
	}
	temp = p_sensor_state->gyro_Ave;
	p_sensor_state->gyro_Ave  = Rolling_Mean( p_control->SampleNumber, p_sensor_state->gyro_Ave, gyro_mag );
	p_sensor_state->gyro_mAve = Windowed_Mean( p_sensor_state->gyro_mAve, gyro_mag, p_control->SampleNumber, 0.005  );
	p_sensor_state->gyro_M2   = Rolling_SumOfSquares( temp, p_sensor_state->gyro_mAve, gyro_mag, p_sensor_state->gyro_M2 );
	p_sensor_state->gyro_sVar = Rolling_Sample_Variance( p_control->SampleNumber, p_sensor_state->gyro_M2 );
	p_sensor_state->gyro_pVar = Rolling_Population_Variance( p_control->SampleNumber, p_sensor_state->gyro_M2 );

  /* Update accel stats */
  accel_mag = Vector_Magnitude ( &p_sensor_state->accel[0] );
  if( p_control->SampleNumber == 1 )
  {
  	p_sensor_state->accel_Ave  = accel_mag;
  	p_sensor_state->accel_mAve = accel_mag;
	  p_sensor_state->accel_M2   = accel_mag;
	  p_sensor_state->accel_sVar = 0.0;
	  p_sensor_state->accel_pVar = 0.0;
  }
	temp = p_sensor_state->accel_Ave;
	p_sensor_state->accel_Ave  = Rolling_Mean( p_control->SampleNumber, p_sensor_state->accel_Ave, accel_mag );
	p_sensor_state->accel_mAve = Windowed_Mean( p_sensor_state->accel_mAve, accel_mag, p_control->SampleNumber, 0.005  );
	p_sensor_state->accel_M2   = Rolling_SumOfSquares( temp, p_sensor_state->accel_mAve, accel_mag, p_sensor_state->accel_M2 );
	p_sensor_state->accel_sVar = Rolling_Sample_Variance( p_control->SampleNumber, p_sensor_state->accel_M2 );
	p_sensor_state->accel_pVar = Rolling_Population_Variance( p_control->SampleNumber, p_sensor_state->accel_M2 );


} /* End Read_Sensors */


/*************************************************
** FUNCTION: delay
** VARIABLES:
**		[I ]	unsigned int mseconds
** RETURN:
**		NONE
** DESCRIPTION:
**		This function emulates the delay function
**		in Arduino.
**		This function is unnecessary and should't
**		be called.
*/
void delay(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}



#endif /* End EXE_MODE (Emulation mode) */
