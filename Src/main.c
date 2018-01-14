
/*******************************************************************
** FILE: main
** This is the calling executable for the emulation
** code. This can only be used offline, and with the
** proper emulation-related flags set appropriatly.
*/


/*******************************************************************
** Includes ********************************************************
********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <inttypes.h>
#include <stdbool.h>

#include "../Include/Emulator_Config.h"
#include "../Include/Emulator_Protos.h"


/*******************************************************************
** Globals *********************************************************
********************************************************************/


/*******************************************************************
** START ***********************************************************
********************************************************************/

int main()
{
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
		WISE_STATE_TYPE   g_wise_state;
	#endif 

	#if( DSP_ON==1 )
		DSP_COMMON_TYPE   g_dsp;
	#endif

	const char* InputFile  = ".\\Data\\BinaryData\\Subject3_2\\F4_2.bin";
	const char* OutputFile = "C:\\Users\\chris\\Desktop\\test.txt";

  float count = 1.0;
  int i,j;
	bool ret;

  g_control.emu_data.InputFID  = fopen(InputFile,"rb");
  g_control.emu_data.OutputFID 	= fopen(OutputFile,"w");
  g_control.emu_data.flag=1;

	/* Initilize the control structure */
  Common_Init( &g_control );
  
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

	GaPA_Init();

	/* Initialize the Directional Cosine Matrix Filter */
  DCM_Init( &g_control, &g_dcm_state, &g_sensor_state );
  
  /* Initialize Walking Incline and Speed Estimator */
  #if( WISE_ON==1 )
  	WISE_Init( &g_control, &g_sensor_state, &g_wise_state );
  #endif

  fprintf(stdout,"pitch:%f\n",g_sensor_state.pitch);
  fprintf(stdout,"Time:%ld\n",g_control_state.timestamp);
	fprintf(stdout,"\n");

  while( g_emu_data.flag==1 )
  {
    /* Update sensor readings */
  	Read_Sensors( &g_control, &g_sensor_state );
  	
  	/* Update the timestamp */
  	Update_Time( &g_control );
  	
  	/* If no data, end */
    if (g_control.emu_data.flag==0); continue;

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
	  
	  /* Estimate the Gait Phase Angle */
    GaPA_Update();
    
	  /* Estimate Walking Speed and Incline */
	  #if( WISE_ON==1 )
	  	if( ((g_dcm_state.gyro_std[0]+g_dcm_state.gyro_std[1]+g_dcm_state.gyro_std[2])/3 > MOVE_MIN_GYRO_STD) )
			{
				WISE_Update(&g_control, &g_sensor_state, &g_wise_state );
			}
		#endif
	

    fprintf(stdout,"\n");
    fprintf(stdout,"Count: %f\n",count);
    fprintf(stdout,"Time:%ld\n",g_control_state.timestamp);

    fprintf(stdout,"pitch:%f\n",g_sensor_state.pitch);

    fprintf(stdout,"nu:%f\n",g_gapa_state.nu);
    fprintf(stdout,"phi:%f\nPHI:%f\n",g_gapa_state.phi, g_gapa_state.PHI);

    fprintf(stdout,"\n");
    //getchar();
    count++;
  }
  
  /* Close the file handles */
  fclose(g_control.emu_data.InputFID);
	//fclose(g_control.emu_data.OutputFID);

  return 0;
} /* End Main */


