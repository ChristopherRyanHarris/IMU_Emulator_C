
/*******************************************************************
** FILE:
**   	main
** DESCRIPTION:
** 		This is the calling executable for the emulation
** 		code. This can only be used offline, and with the
** 		proper emulation-related flags set appropriatly.
********************************************************************/


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

#include "../Include/Common_Config.h"
#include "../Include/Emulator_Protos.h"

#ifdef _IMU10736_
	#include "../Include/IMU10736_Config.h"
#endif
#ifdef _IMU9250_
	#include "../Include/IMU9250_Config.h"
#endif

/*******************************************************************
** Globals *********************************************************
********************************************************************/


/*******************************************************************
** START ***********************************************************
********************************************************************/

int main( void )
{
	/************************************************************
	** ------------- Declare Data Structures --------------------
	** In the first block we must do quite a bit of trickery in
	** in order to cleanly initialize all the structures in a
	** way which both allows room for easy modification and
	** properly prepares for the execution.
	** Since the desire is to have this code be used to develop
	** potentially many algorithms, I decided to initially use
	** a control architecture to turn the various programs on
	** or off.
	** Further, since this code is eventually ported to an
	** arduino implementation, I wanted to be sure not to fill
	** up the memory with unused code.
	** This method could be easily changed later to allow for
	** the various streams to be hot-swaped.
	************************************************************/

	/* Control Structure
	** This will contain all the various settings */
	CONTROL_TYPE        g_control;

	/* Input data structure
	** This will contain the input data
	** In emulation mode, this is read from a binary file
	** In real-time mode, this will be from the sensors. */
	SENSOR_STATE_TYPE   g_sensor_state;

	/* Calibration Structure
	** This structure is used to aid in calibrating the sensor
	** This is not used in normal processing */
	CALIBRATION_TYPE  g_calibration;


	/* DSP state
	** The Digitial Signal Processing algorithms
	** are filters which are applied to the individual
	** data feeds. This structure holds the state variables
	** for the algorithm.
	** NOTE: At the moment, this is a very simple FIR and IIR
	**			 filter set. Could easily be expanded. */
	DSP_STATE_TYPE   g_dsp;


	/* DCM variables
	** The Directional cosine matrix is one
	** method of determining the orientation of the
	** IMU. This matrix holds the state information of
	** the DCM algorihtm. */
	DCM_STATE_TYPE      g_dcm_state;


	/* GaPA state
	** The Gait Phase Angle estimator is an aglorithm
	** (or set of algorithms) which will determine the
	** current gait phase angle of the user. This
	** structure holds the state variables of the algorithm */
	GAPA_STATE_TYPE   g_gapa_state;


	/* WISE state
	** The Walking Incline and Speed Estimator is
	** an algorithm (or set of algorithms) which
	** estimates the walking speed and the incline
	** of motion of the user. This structure holds
	** the state variables for the algorithm. */
	WISE_STATE_TYPE   g_wise_state;




	/************************************************************
	** ---------------- Initialize Variables --------------------
	** Note that we must get an intial read of all the active
	** sensors before we initialize the data structures.
	************************************************************/

	/* Set input data file and (optional) output file */
	g_control.emu_data.InputFile  = ".\\Data\\BinaryData\\Subject3_2\\F4_2.bin";
	g_control.emu_data.OutputFile = "C:\\Users\\chris\\Desktop\\test.txt";

  float count = 1.0;
	bool ret;
	char junk;

	/* Initilize the control structure */
  Common_Init( &g_control );

	/* Open input file */
  g_control.emu_data.InputFID = fopen(g_control.emu_data.InputFile,"rb");
  if( g_control.emu_data.InputFID==NULL )
  {
  	fprintf(stderr,"ERROR : Opening Input Data file %s : File Handle Null",g_control.emu_data.InputFile);
  }

  /* If desired, open output file to write debug data
  ** TODO: This could easily be expanded to be an execution log file */
  //g_control.emu_data.OutputFID 	= fopen(OutputFile,"w");
	//if( g_control.emu_data.InputFID==NULL )
  //{
  //	fprintf(stderr,"ERROR : Opening Input Data file %s : File Handle Null",InputFile);
  //}

  /* Read all active sensors */
  Read_Sensors( &g_control, &g_sensor_state );

  /* Initialize Freq. Filter */
  if( g_control.DSP_on==1 ){ DSP_Filter_Init( &g_control, &g_dsp ); }

	/* Initialize calibartion parameters */
  if( g_control.calibration_on==1 ){ Calibration_Init( &g_control, &g_calibration ); }

	/* Initialize GaPA parameters */
  if( g_control.GaPA_on==1 ){ GaPA_Init( &g_control, &g_gapa_state ); }

	/* Initialize the Directional Cosine Matrix algorithm parameters */
  if( g_control.DCM_on==1 ){ DCM_Init( &g_control, &g_dcm_state, &g_sensor_state ); }

  /* Initialize Walking Incline and Speed Estimator */
  if( g_control.WISE_on==1 ){ WISE_Init( &g_control, &g_sensor_state, &g_wise_state ); }



  fprintf(stdout,"pitch:%f\n",g_sensor_state.pitch);
  fprintf(stdout,"Time:%ld\n",g_control.timestamp);
	fprintf(stdout,"\n");

	/************************************************************
	** -------------------- Loop Begin --------------------------
	************************************************************/

	/* We execute until EOF */
  while( (junk=getc(g_control.emu_data.InputFID)) != EOF )
  {
  	/* Rewind getc (from above) */
  	fseek(g_control.emu_data.InputFID,-1L,SEEK_CUR);

    /* Update sensor readings */
  	Read_Sensors( &g_control, &g_sensor_state );

  	/* Update the timestamp */
  	Update_Time( &g_control );

    /* If in calibration mode,
		** call calibration function */
	  if( g_control.calibration_on==1 ){ Calibrate( &g_control, &g_calibration, &g_sensor_state ); }

	  /* Apply Freq Filter to Input */
	  if( g_control.DSP_on==1 )
  	{
	  	if( g_control.dsp_prms.IIR_on==1 ){ FIR_Filter( &g_control, &g_dsp, &g_sensor_state ); }
	  	if( g_control.dsp_prms.IIR_on==1 ){ IIR_Filter( &g_control, &g_dsp, &g_sensor_state ); }
	  	DSP_Shift( &g_control, &g_dsp );
	  }

	  /* Apply the DCM Filter */
	  if( g_control.DCM_on==1 ){ DCM_Filter( &g_control, &g_dcm_state, &g_sensor_state ); }

	  /* Estimate the Gait Phase Angle */
	  if( g_control.GaPA_on==1 ){ GaPA_Update( &g_control, &g_sensor_state, &g_gapa_state ); }

	  /* Estimate Walking Speed and Incline */
	  if( g_control.WISE_on==1 )
	  {
	  	if( ((g_dcm_state.gyro_std[0]+g_dcm_state.gyro_std[1]+g_dcm_state.gyro_std[2])/3 > MOVE_MIN_GYRO_STD) )
			{
				WISE_Update(&g_control, &g_sensor_state, &g_wise_state );
			}
		}


    fprintf(stdout,"\n");
    fprintf(stdout,"Count: %f\n",count);
    fprintf(stdout,"Time:%ld\n",g_control.timestamp);

    fprintf(stdout,"pitch:%f\n",g_sensor_state.pitch);

    fprintf(stdout,"nu:%f\n",g_gapa_state.nu);
    fprintf(stdout,"phi:%f\nPHI:%f\n",g_gapa_state.phi, g_gapa_state.PHI);

    fprintf(stdout,"\n");

    getchar();

    count++;
  }

  /* Close the file handles */
  if( g_control.emu_data.InputFID!=NULL ){ fclose(g_control.emu_data.InputFID); }

  if( g_control.emu_data.OutputFID!=NULL ){ fclose(g_control.emu_data.OutputFID); }


  return 0;
} /* End Main */


