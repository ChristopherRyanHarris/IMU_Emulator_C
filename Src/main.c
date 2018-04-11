
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
	** the various streams to be hot-swapped.
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
	** The Digital Signal Processing algorithms
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
	** the DCM algorithm. */
	DCM_STATE_TYPE      g_dcm_state;


	/* GaPA state
	** The Gait Phase Angle estimator is an algorithm
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
	** Note that we must get an initial read of all the active
	** sensors before we initialize the data structures.
	************************************************************/

	/* Set input data file and (optional) output file */
	//g_control.emu_data.InputFile  = ".\\Data\\BinaryData\\Subject3_2\\F4_2.bin";
  g_control.emu_data.InputFile  = "F:\\Research_Data\\Original\\Subject3_3\\Subject3_04040.bin";

	g_control.emu_data.OutputFile = "F:\\MatlabWorkspace\\C Testing\\C_testing_2.bin";

  long unsigned int count = 0;
  //long long unsigned int count = 0;
  //float count = 0;
	bool ret;
	char junk;

	/* Used in cases I want to write the logs out to
	** a file rather than stdout */
  //SET_EMULATION_stdout(stdout);

	/* Initialize the control structure */
  Common_Init( &g_control, &g_sensor_state );

	/* Open input file */
  g_control.emu_data.InputFID = fopen(g_control.emu_data.InputFile,"rb");
  if( g_control.emu_data.InputFID==NULL )
  {
  	fprintf(stderr,"ERROR : Opening Input Data file %s : File Handle Null",g_control.emu_data.InputFile);
  }

  /* If desired, open output file to write debug data
  ** TODO: This could easily be expanded to be an execution log file */
  g_control.emu_data.OutputFID 	= fopen(g_control.emu_data.OutputFile,"wb");
	if( g_control.emu_data.OutputFID==NULL )
  {
  	fprintf(stderr,"ERROR : Opening Output Data file %s : File Handle Null",g_control.emu_data.OutputFile);
  }

	LOG_PRINTLN("Input File : %s",g_control.emu_data.InputFile);
	LOG_PRINTLN("Output File : %s",g_control.emu_data.OutputFile);

  /* Read all active sensors */
  Read_Sensors( &g_control, &g_sensor_state );

  /* Initialize Freq. Filter */
  if( g_control.DSP_on==1 ){ DSP_Filter_Init( &g_control, &g_dsp ); }

	/* Initialize calibration parameters */
  if( g_control.calibration_on==1 ){ Calibration_Init( &g_control, &g_calibration ); }

	/* Initialize the Directional Cosine Matrix algorithm parameters */
  if( g_control.DCM_on==1 ){ DCM_Init( &g_control, &g_dcm_state, &g_sensor_state ); }

	/* Initialize GaPA parameters */
  if( g_control.GaPA_on==1 ){ GaPA_Init( &g_control, &g_gapa_state ); }

  /* Initialize Walking Incline and Speed Estimator */
  if( g_control.WISE_on==1 ){ WISE_Init( &g_control, &g_sensor_state, &g_wise_state ); }

  LOG_PRINTLN("sizeof(count) : %d", sizeof(count));
  LOG_PRINTLN("sizeof(g_control.timestamp) : %d", sizeof(g_control.timestamp));
  LOG_PRINTLN("sizeof(g_control.G_Dt) : %d", sizeof(g_control.G_Dt));
  LOG_PRINTLN("sizeof(g_sensor_state.pitch) : %d", sizeof(g_sensor_state.pitch));
  LOG_PRINTLN("sizeof(g_gapa_state.phi) : %d", sizeof(g_gapa_state.phi));
  LOG_PRINTLN("sizeof(g_gapa_state.PHI) : %d", sizeof(g_gapa_state.PHI));
  LOG_PRINTLN("sizeof(g_gapa_state.nu) : %d", sizeof(g_gapa_state.nu));
  //getchar();

  fwrite( &(count), 1, sizeof(count), g_control.emu_data.OutputFID );
  fwrite( &(g_control.timestamp), 1, sizeof(g_control.timestamp), g_control.emu_data.OutputFID );
  fwrite( &(g_control.G_Dt), 1, sizeof(g_control.G_Dt), g_control.emu_data.OutputFID );
  fwrite( &(g_sensor_state.pitch), 1, sizeof(g_sensor_state.pitch), g_control.emu_data.OutputFID );
  fwrite( &(g_gapa_state.phi), 1, sizeof(g_gapa_state.phi), g_control.emu_data.OutputFID );
  fwrite( &(g_gapa_state.PHI), 1, sizeof(g_gapa_state.PHI), g_control.emu_data.OutputFID );
  fwrite( &(g_gapa_state.nu_normalized), 1, sizeof(g_gapa_state.nu_normalized), g_control.emu_data.OutputFID );
	fwrite( &(g_sensor_state.gyro_mAve), 1, sizeof(g_sensor_state.gyro_mAve), g_control.emu_data.OutputFID );
  fwrite( &(g_gapa_state.z_phi), 1, sizeof(g_gapa_state.z_phi), g_control.emu_data.OutputFID );
  fwrite( &(g_gapa_state.z_PHI), 1, sizeof(g_gapa_state.z_PHI), g_control.emu_data.OutputFID );

  //fprintf(g_control.emu_data.OutputFID,"%f\n",g_sensor_state.pitch);
  //fprintf(g_control.emu_data.OutputFID,"%lu\n",g_control.emu_data.timestamp);
  //fprintf(stdout,"Time:%ld\n",g_control.timestamp);
	//fprintf(stdout,"\n");

	/************************************************************
	** -------------------- Loop Begin --------------------------
	************************************************************/

	/* We execute until EOF */
	count = 1;
  while( getc(g_control.emu_data.InputFID) != EOF )
  {
  	/* Rewind getc (from above) */
  	fseek(g_control.emu_data.InputFID,-1L,SEEK_CUR);

    /* Update sensor readings */
  	Read_Sensors( &g_control, &g_sensor_state );

  	/* Update the timestamp */
  	Update_Time( &g_control );

  	//LOG_PRINTLN("(%ld) TIME:%lu DT:%f",count, g_control.timestamp, g_control.G_Dt);

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
	  	if( (g_sensor_state.gyro_mAve<g_control.gapa_prms.min_gyro) )
			{
				WISE_Update(&g_control, &g_sensor_state, &g_wise_state );
			}
		}

    //LOG_PRINTLN("Pitch:%f", g_sensor_state.pitch);
    //LOG_PRINTLN("phi:%6.3f PHI:%6.3f Nu:%6.3f", g_gapa_state.phi, g_gapa_state.PHI, g_gapa_state.nu);


    fwrite( &(count), 1, sizeof(count), g_control.emu_data.OutputFID );
    fwrite( &(g_control.timestamp), 1, sizeof(g_control.timestamp), g_control.emu_data.OutputFID );
    fwrite( &(g_control.G_Dt), 1, sizeof(g_control.G_Dt), g_control.emu_data.OutputFID );
    fwrite( &(g_sensor_state.pitch), 1, sizeof(g_sensor_state.pitch), g_control.emu_data.OutputFID );
    fwrite( &(g_gapa_state.phi), 1, sizeof(g_gapa_state.phi), g_control.emu_data.OutputFID );
    fwrite( &(g_gapa_state.PHI), 1, sizeof(g_gapa_state.PHI), g_control.emu_data.OutputFID );
    fwrite( &(g_gapa_state.nu_normalized), 1, sizeof(g_gapa_state.nu_normalized), g_control.emu_data.OutputFID );
    fwrite( &(g_sensor_state.gyro_mAve), 1, sizeof(g_sensor_state.gyro_mAve), g_control.emu_data.OutputFID );
    fwrite( &(g_gapa_state.z_phi), 1, sizeof(g_gapa_state.z_phi), g_control.emu_data.OutputFID );
    fwrite( &(g_gapa_state.z_PHI), 1, sizeof(g_gapa_state.z_PHI), g_control.emu_data.OutputFID );

    //getchar();

    count++;
  }

  /* Close the file handles */
  if( g_control.emu_data.InputFID!=NULL ){ fclose(g_control.emu_data.InputFID); }

  if( g_control.emu_data.OutputFID!=NULL ){ fclose(g_control.emu_data.OutputFID); }


  return 0;
} /* End Main */


