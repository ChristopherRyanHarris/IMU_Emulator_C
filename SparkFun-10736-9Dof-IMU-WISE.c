
#include <Wire.h>
#include <string.h>
#include <math.h>

#include "IMU10736_Config.h"
#include "Common_Config.h"
#include "DSP_Config.h"
#include "WISE_Config.h"
#include "Math.h"


/*******************************************************************
** Globals *********************************************************
********************************************************************/

/* DCM variables */
CAL_STATE_TYPE      g_calibration;
DCM_STATE_TYPE      g_dcm_state;
DSP_COMMON_TYPE     g_dsp;
SENSOR_STATE_TYPE   g_sensor_state;
CONTROL_STATE_TYPE  g_control_state;
SWE_STATE_TYPE      g_swe_state;


/*******************************************************************
** START ***********************************************************
********************************************************************/

/*************************************************
** Setup Function 
** This function contains the setup functions 
** including the initialization of the hardware
** and the initialization of the serial ports
*/
void setup()
{
  /* Initialize the hardware */
  Init_Hardware();

  /* Initialize the IMU */
  if ( !Init_IMU() ) 
  {
    LOG_PRINTLN("Error connecting to IMU");
    while(1){ }  // Loop forever if we fail to connect
  }
  LOG_PRINTLN("> IMU Initialized");
  delay(20);
  
  /* Set the initial roll/pitch/yaw from 
  ** initial accel/gyro */
  Reset_Sensor_Fusion(); 
  
  if( CALIBRATE_MODE ) { Calibration_Init(); }
  
  SWE_Init();
  DSP_Filter_Init();
}

// Main loop
void loop()
{ 
  /* Update sensor readings */
  Read_Sensors();
  
  /* Apply Freq Filter to Input */
  //FIR_Filter();
  //IIR_Filter();
  //DSP_Shift();

  if( CALIBRATE_MODE ) { Calibrate(); }
  
  /* Apply the DCM Filter */
  Update_Time();
  DCM_Filter();
  
  /* Estimate Walking Speed and Incline */
  SWE_Update();

  /* Read/Respond to command */
  if( Serial.available() > 0 ) { f_SendData( Serial.available() );  }

  /* Blink LED 
  ** TO DO: It would be nice to have a blink code
  **        to communicate during operation */
  Blink_LED();
}






