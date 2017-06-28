/*************************************************
** FILE: CAL_Functions
** This file contains the functions which will help
** calibration the sensors
**************************************************/

#include "Common_Config.h"
#if EXE_MODE /* Emulator mode */

#include "Emulator_Config.h"
extern CAL_STATE_TYPE      g_calibration;
extern DCM_STATE_TYPE      g_dcm_state;
extern DSP_COMMON_TYPE     g_dsp;
extern SENSOR_STATE_TYPE   g_sensor_state;
extern CONTROL_STATE_TYPE  g_control_state;
extern WISE_STATE_TYPE     g_wise_state;
#endif

/*
** Function: Calibration_Init
** This function initializes the calibration state
** variables.
*/
void Calibration_Init ( void )
{
  int i;
  for( i=0; i<3; i++ )
  {
    g_calibration.accel_total[i] = 0.0f;
    g_calibration.accel_max[i]   = -9999.0f;
    g_calibration.accel_min[i]   = 9999.0f;

    g_calibration.gyro_total[i] = 0.0f;
    g_calibration.gyro_max[i]   = -9999.0f;
    g_calibration.gyro_min[i]   = 9999.0f;
  }
  g_calibration.N = 0;
}



/*
** Function: Calibrate
*/
void Calibrate ( void )
{
  int i;
  for( i=0; i<3; i++ )
  {
    g_calibration.accel_total[i] += g_sensor_state.accel[i];
    if( g_sensor_state.accel[i] > g_calibration.accel_max[i] ) { g_calibration.accel_max[i] = g_sensor_state.accel[i]; }
    if( g_sensor_state.accel[i] < g_calibration.accel_min[i] ) { g_calibration.accel_min[i] = g_sensor_state.accel[i]; }

    g_calibration.gyro_total[i] += g_sensor_state.gyro[i];
    if( g_sensor_state.gyro[i] > g_calibration.gyro_max[i] ) { g_calibration.gyro_max[i] = g_sensor_state.gyro[i]; }
    if( g_sensor_state.gyro[i] < g_calibration.gyro_min[i] ) { g_calibration.gyro_min[i] = g_sensor_state.gyro[i]; }
  }

  g_calibration.N++;
}









