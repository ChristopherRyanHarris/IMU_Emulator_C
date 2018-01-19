
/*******************************************************************
** FILE: 
**   	IMU9250_Functions
** DESCRIPTION:
** 		This file contains the helper functions exclusive to running
** 		emulation mode. These functions are implemented separatly
** 		(or not at all) in real-time mode.
********************************************************************/


/*******************************************************************
** Includes ********************************************************
********************************************************************/


#include "../Include/Common_Config.h"
#if EXE_MODE==1 /* Emulator Mode */
	#include "../Include/Emulator_Config.h"
	#include "../Include/Emulator_Protos.h"
	#include "../Include/Math.h"

	#ifdef _IMU10736_
		#include "../Include/IMU10736_Config.h"
	#endif
	#ifdef _IMU9250_
		#include "../Include/IMU9250_Config.h"
	#endif

#endif /* End Emulator Mode */


/*******************************************************************
** Functions *******************************************************
********************************************************************/

/*************************************************
** Init_Emulator
*/
void Init_Emulator( CONTROL_TYPE *p_control )
{
  LOG_PRINT("> Initializing\n");

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
**		[I ]	CONTROL_TYPE		*p_control
**		[IO]	DSP_COMMON_TYPE	*p_dsp_state
** RETURN:
**		NONE
** DESCRIPTION:
*/
void Read_Sensors( CONTROL_TYPE				*p_control,
								 	 SENSOR_STATE_TYPE	*p_sensor_state )
{
  float temp;
  int   count;

  count = fread(&temp,sizeof(float),1,p_control->emu_data.InputFID);
  if ( count != 1 )
  {
    fprintf(stdout,"Emulator Exit\n");
    p_control->emu_data.flag = 0;
    return;
  }

  //Format:1
  // Data Colleciton: 1
  // Format:
  //   14x[float32] 14x[float32] ....
  //   [Timestamp][accel1][accel2][accel3][gyro1][gyro2][gyro3][yaw][pitch][roll][aest_ave1][aest_ave2][vest_ave1][vest_ave2]
  //p_control->emu_data.timestamp = (unsigned long) temp;
  //fread(&p_sensor_state->accel[0],sizeof(float),3,p_control->emu_data.InputFID);
  //fread(&p_sensor_state->gyro[0],sizeof(float),3,p_control->emu_data.InputFID);
  //fseek(p_control->emu_data.InputFID,sizeof(float)*7,SEEK_CUR);


  // Format: 2
  // Data collecion: 2
  //   10x[float32] 10x[float32] ...
  //   [1]        [2]     [3]     [4]     [5]    [6]    [7]    [8]  [9]    [10]
  //   [Timestamp][accel1][accel2][accel3][gyro1][gyro2][gyro3][yaw][pitch][roll]
  p_control->emu_data.timestamp = (unsigned long) temp;
  fread(&p_sensor_state->accel[0],sizeof(float),3,p_control->emu_data.InputFID);
  fread(&p_sensor_state->gyro[0],sizeof(float),3,p_control->emu_data.InputFID);
  fseek(p_control->emu_data.InputFID,sizeof(float)*3,SEEK_CUR);

} /* End Read_Sensors */




