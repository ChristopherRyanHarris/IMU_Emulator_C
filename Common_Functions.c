
/*************************************************
** FILE: Common_Functions
** This file contains some MPU 9250 (HW specific)
** functions. Specifically, for initializing and
** reading the sensor registeres
**************************************************/

#include "Common_Config.h"
#if EXE_MODE /* Emulator mode */
#include "DSP_Lib/DSP_Config.h"
#include "WISE_Lib/WISE_Config.h"
#include "IMU10736_Lib/IMU10736_Config.h"

#include "Emulator_Config.h"
extern CAL_STATE_TYPE      g_calibration;
extern DCM_STATE_TYPE      g_dcm_state;
extern DSP_COMMON_TYPE     g_dsp;
extern SENSOR_STATE_TYPE   g_sensor_state;
extern CONTROL_STATE_TYPE  g_control_state;
extern WISE_STATE_TYPE     g_wise_state;
#endif


/* Common_Init
** This function initializes variables and constants which
** are the same across all platforms and which are common
** across all agorithm variants
*/
void Common_Init( void )
{
  LOG_PRINT("> Initializing\n");

  g_calibration.output_mode    = CAL_OUTPUT_MODE;
  //g_calibration.calibrate_flag = 0;

  g_control_state.timestamp      = 0;
  g_control_state.timestamp_old  = 0;
  g_control_state.G_Dt           = 0.0;

  g_control_state.g_BaudLock       = true;
  g_control_state.g_LedState       = false;
  g_control_state.g_LastBlinkTime  = 0;
}

