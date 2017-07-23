
/*************************************************
** FILE: SparkFun-9DoF-IMU-WISE
** This is the calling executable for the real-time
** Sparkfun IMU. This specific implementation is
** designed for the WISE (Walking Incline and Speed
** Estimator) algorithm variant. To use, this code
** must be embedded into a compatible IMU and the
** appropriate header files must be referenced.
*/


/*******************************************************************
** Includes ********************************************************
********************************************************************/


#include "../Include/Common_Config.h"

#if EXE_MODE==1 /* Emulator Mode */
#include "../Include/Emulator_Config.h"
extern CAL_STATE_TYPE      g_calibration;
extern DCM_STATE_TYPE      g_dcm_state;
extern DSP_COMMON_TYPE     g_dsp;
extern SENSOR_STATE_TYPE   g_sensor_state;
extern CONTROL_STATE_TYPE  g_control_state;
extern WISE_STATE_TYPE     g_wise_state;
extern EMULATE_TYPE        g_emu_data;

extern const char* g_EmuFile;
extern FILE*       g_fid;
#endif /* End Emulator Mode */


/*******************************************************************
** Functions *******************************************************
********************************************************************/

/*************************************************
** Init_Emulator
*/
void Init_Emulator( void )
{
  LOG_PRINT("> Initializing\n");

  g_control_state.timestamp      = 0;
  g_control_state.timestamp_old  = 0;
  g_control_state.G_Dt           = 0.0;

  g_control_state.g_BaudLock       = true;
  g_control_state.g_LedState       = false;
  g_control_state.g_LastBlinkTime  = 0;
} /* End Init_Emulator */


/*************************************************
** Read_Sensors
*/
void Read_Sensors( void )
{
  float temp;
  int   count;

  count = fread(&temp,sizeof(float),1,g_fid);
  if ( count != 1 )
  {
    fprintf(stdout,"Emulator Exit\n");
    g_emu_data.flag = 0;
    return;
  }

  g_emu_data.timestamp = (unsigned long) temp;
  fread(&g_sensor_state.accel[0],sizeof(float),3,g_fid);
  fread(&g_sensor_state.gyro[0],sizeof(float),3,g_fid);
  fseek(g_fid,sizeof(float)*7,SEEK_CUR);
} /* End Read_Sensors */




