
/*******************************************************************
** FILE:
**    Common_Functions
** DESCRIPTION:
**    This file contains several setup and initialization functions
**    which are common across all execution platforms.
**    Any functions or algorithms added to this file should be common
**    to all platforms and algorithms.
********************************************************************/


/*******************************************************************
** Includes ********************************************************
********************************************************************/

#ifndef COMMON_CONFIG_H
  #include "../Include/Common_Config.h"
#endif
#if EXE_MODE==1 /* Emulator Mode */
  /* In emulation mode, "Emulator_Protos" is needed to
  ** use functions in other files.
  ** NOTE: This header should contain the function
  **       prototypes for all execution functions */
  #include "../Include/Emulator_Protos.h"
#endif  /* End Emulator Mode */

/*******************************************************************
** Functions *******************************************************
********************************************************************/


/*************************************************
** FUNCTION: Common_Init
** VARIABLES:
**    [IO]  CONTROL_TYPE  *p_control
**    [IO]
** RETURN:
**    NONE
** DESCRIPTION:
**    This function initializes variables and constants which
**    are the same across all platforms and which are common
**    across all algorithm variants
*/
void Common_Init ( CONTROL_TYPE       *p_control,
                   SENSOR_STATE_TYPE  *p_sensor_state)
{
  //char buffer[50];

  LOG_INFO( "> Initializing Common Parameters");

  /* Initialize sample counter */
  p_control->SampleNumber         = 0;
  p_control->SampleNumberOverflow = 0;

  /* Set default IO mode */
  p_control->output_mode = OUTPUT_MODE;

  /* Set common exe parameters */
  p_control->timestamp      = 0;
  p_control->timestamp_old  = 0;
  p_control->G_Dt           = 0.0;

  /* For emulation mode,
  ** am "emu timestamp" is needed  */
  #if EXE_MODE==1 /* Emulator Mode */
    p_control->emu_data.timestamp = 0;
    p_control->emu_data.InputFID  = NULL;
    p_control->emu_data.OutputFID = NULL;
  #endif

  p_control->verbose = DEBUG;

  /* Set file parameters */
  /* TO DO : Clean this up */
  strcpy( p_control->log_info_file.file_prefix, LOG_INFO_FILE_PREFIX );
  strcpy( p_control->log_info_file.file_suffix, LOG_INFO_FILE_SUFFIX );
  strcpy( p_control->log_data_file.file_prefix, LOG_DATA_FILE_PREFIX );
  strcpy( p_control->log_data_file.file_suffix, LOG_DATA_FILE_SUFFIX );

  #if( EXE_MODE==0 ) /* 0 : IMU Mode */
    if( ENABLE_SD_LOGGING==TRUE )
    {
      LOG_INFO( " > SD Logging enabled");

      p_control->SDCardPresent = (SD.begin(SD_PIN));
      p_control->LogBufferLen  = 0;
      p_control->LogBuffer[0]  = '\0';
      if( p_control->SDCardPresent==TRUE )
      {
        LOG_INFO( " > SD Card Detected");

        p_control->LogFileIdx = 0;
        GetNextLogFileName( p_control );
        LOG_INFO( " > Using File %s", p_control->LogFileName);

        p_control->LogFile_fh = SD.open( p_control->LogFileName, FILE_WRITE );
        if( p_control->LogFile_fh!=NULL )
        {
          LOG_INFO( " > Open Successful");
          //p_control->LogFile_fh.close();
          // p_control->LogFile_fh=SD.open( p_control->LogFileName, FILE_WRITE );
          //      p_control->LogFile_fh.close();
        }
        else
        {
          LOG_INFO( " > Open Failed, Disabline SD Logging" );
          p_control->SDCardPresent = FALSE;
        }
      }
      else
      {
        LOG_INFO( " > SD Card Not Detected, Disabling SD Logging" );
      }
    }
  #else
    /* TO DO : Add emulator file open calls here */
  #endif

  p_control->calibration_on = CALIBRATION_MODE;
  p_control->DSP_on         = DSP_ON;
  p_control->DCM_on         = DCM_ON;
  p_control->GaPA_on        = GAPA_ON;
  p_control->WISE_on        = WISE_ON;

  /* Set mode parameters */
  p_control->sensor_prms.gravity     = GRAVITY;
  p_control->sensor_prms.accel_on    = ACCEL_ON;
  p_control->sensor_prms.gyro_on     = GYRO_ON;
  p_control->sensor_prms.magn_on     = MAGN_ON;
  p_control->sensor_prms.sample_rate = TIME_SR;

  /* Initialize stats */
  p_sensor_state->gyro_Ave  = 0.0;
  p_sensor_state->gyro_mAve = 0.0;
  p_sensor_state->gyro_M2   = 0.0;
  p_sensor_state->gyro_sVar = 0.0;
  p_sensor_state->gyro_pVar = 0.0;

  p_sensor_state->accel_Ave  = 0.0;
  p_sensor_state->accel_mAve = 0.0;
  p_sensor_state->accel_M2   = 0.0;
  p_sensor_state->accel_sVar = 0.0;
  p_sensor_state->accel_pVar = 0.0;

} /* End Common_Init*/


/*************************************************
** FUNCTION: UpdateTime
** VARIABLES:
**    [IO]  CONTROL_TYPE  *p_control
** RETURN:
**    NONE
** DESCRIPTION:
**    Update the time state
**    Delta time (s) is used to determine the state
**    estimate in the filter.
*/
void Update_Time( CONTROL_TYPE *p_control )
{

  #if EXE_MODE==1 /* Emulator Mode */
    /* Timestamp is read from file */
    p_control->timestamp_old = p_control->timestamp;
    p_control->timestamp     = p_control->emu_data.timestamp;
    LOG_INFO("TIME:%lu", p_control->timestamp );
  #else /* Real Time mode */
    float minTime = (float) (TIME_RESOLUTION / (TIME_SR+1.0) ); /* Set Sampling Rate */
    while( (TIME_FUPDATE - p_control->timestamp) < (minTime) ) {}
    /* Update delta T */
    p_control->timestamp_old = p_control->timestamp;
    p_control->timestamp     = TIME_FUPDATE;

  #endif /* End Emulator Mode */

  /* Get delta t */
  if( p_control->timestamp_old > 0 )
  {
    p_control->G_Dt = (float) ( (p_control->timestamp - p_control->timestamp_old) / TIME_RESOLUTION ) ;
  }
  else
  {
    p_control->G_Dt = 0.0f;
  }
} /* End Update_Time */

