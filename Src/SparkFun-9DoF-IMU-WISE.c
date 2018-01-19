
/*******************************************************************
** FILE: 
**		SparkFun-9DoF-IMU-WISE
** DESCRIPTION:
** 		This is the calling executable for the real-time
** 		Sparkfun IMU code.
** 		To use, this code must be embedded into a compatible IMU and the 
** 		appropriate header files must be referenced.
** 		See the common header file for more specific information.
********************************************************************/


/*******************************************************************
** Includes ********************************************************
********************************************************************/

#include "../Include/Common_Config.h"

#include <Wire.h>
#include <string.h>
#include <math.h>

#ifdef _IMU10736_
	#include "../Include/IMU10736_Config.h"
#endif

#ifdef _IMU9250_
	#include <SparkFunMPU9250-DMP.h>
	#include "../Include/IMU9250_Config.h"
#endif

#include "../Include/DCM_Config.h"
#include "../Include/DSP_Config.h"
#include "../Include/WISE_Config.h"
#include "../Include/Math.h"


/*******************************************************************
** Globals *********************************************************
********************************************************************/

#ifdef _IMU9250_
	MPU9250_DMP imu; 
#endif

/* Control structure */
CONTROL_TYPE        g_control;
SENSOR_STATE_TYPE   g_sensor_state;

/* Calibration Structure */
#if CALIBRATION_MODE==1
	CALIBRATION_TYPE  g_calibration;
#endif

/* DCM variables */
DCM_STATE_TYPE      g_dcm_state;

#if( WISE_ON==1 )
	WISE_STATE_TYPE     g_wise_state;
#endif 

#if( DSP_ON==1 )
	DSP_COMMON_TYPE     g_dsp;
#endif

/*******************************************************************
** START ***********************************************************
********************************************************************/


/*************************************************
** FUNCTION: setup
** VARIABLES:
**		NONE
** RETURN:
**		NONE
** DESCRIPTION:
** 		This function contains the setup functions 
** 		including the initialization of the hardware
** 		and the initialization of the serial ports
*/
void setup( void )
{
	bool ret;
	
	/* Initialize the hardware */
  #if EXE_MODE==0 /* IMU real-time Mode */
  	Init_Hardware( &g_control );
  #endif
  
	/* Initilize the control structure */
  Common_Init( &g_control );
  
  /* Initialize the IMU sensors*/
  #if EXE_MODE==0 /* IMU real-time Mode */
  	ret = Init_IMU( &g_control, &g_sensor_state );
  	if ( ret==0 ) 
  	{
    	LOG_PRINTLN("ERROR : Setup : Cant Connect to IMU");
    	while(1){}
  	}
  	delay(2000);
  #endif
  
  /* Set the initial roll/pitch/yaw from 
  ** initial accel/gyro */
  
  /* Read all active sensors */
  Read_Sensors( &g_control, &g_sensor_state );
  
  /* Initialize Freq. Filter */
  #if( DSP_ON==1 )
  	DSP_Filter_Init( &g_control, &g_dsp );
  #endif
  
  #if CALIBRATE_MODE==1
  	Calibration_Init( &g_calibration );
  #endif

	/* Initialize the Directional Cosine Matrix Filter */
  DCM_Init( &g_control, &g_dcm_state, &g_sensor_state );
  
  /* Initialize Walking Incline and Speed Estimator */
  #if( WISE_ON==1 )
  	WISE_Init( &g_control, &g_sensor_state, &g_wise_state );
  #endif
  
  LOG_PRINTLN("> IMU Setup Done");
  
} /* End setup */


/*************************************************
** FUNCTION: loop
** VARIABLES:
**		NONE
** RETURN:
**		NONE
** DESCRIPTION:
**		In the case of IMU real-time execution, 
**		this is the main loop for the executable.
**		It loops while there is power.		
*/
void loop( void )
{ 
  /* Update sensor readings */
  Read_Sensors( &g_control, &g_sensor_state );
  
  /* Update the timestamp */
  Update_Time( &g_control );
  
  /* Apply Freq Filter to Input */
  #if( DSP_ON==1 )
  	FIR_Filter( &g_control, &g_dsp, &g_sensor_state );
  	IIR_Filter( &g_control, &g_dsp, &g_sensor_state );
  	DSP_Shift( &g_control, &g_dsp );
  #endif

	/* If in calibration mode, 
	** call calibration function */
  #if( CALIBRATE_MODE==1 )
  	Calibrate( &g_control, &g_calibration, &g_sensor_state );
  #endif
  
  /* Apply the DCM Filter */
  DCM_Filter( &g_control, &g_dcm_state, &g_sensor_state );
  
  /* Estimate Walking Speed and Incline */
  #if( WISE_ON==1 )
  	if( ((g_dcm_state.gyro_std[0]+g_dcm_state.gyro_std[1]+g_dcm_state.gyro_std[2])/3 > MOVE_MIN_GYRO_STD) )
		{
			WISE_Update( &g_control, &g_sensor_state, &g_wise_state );
		}
	#endif
    
  /* Read/Respond to command */
  if( COMM_PORT.available()>0 ) { f_RespondToInput( COMM_PORT.available() );  }

  /* We blink every UART_LOG_RATE millisecods */
  if ( micros()>(g_control_state.LastLogTime+UART_LOG_RATE) )
  {
  	/* Log the current states to the debug port */
    Debug_LogOut( &g_control, &g_sensor_state, &g_wise_state );
    
    g_control_state.g_LastLogTime = micros();

    /* Display number of bytes available on comm port
    ** Com port is used for real-time communication with
    ** connected processor */
    //LOG_PORT.println("> # Available on COMM_PORT: " + String(COMM_PORT.available()) );
  }

  /* Blink LED 
  ** TO DO: It would be nice to have a blink code
  **        to communicate during operation */
  Blink_LED( &g_control );
} /* End loop */






