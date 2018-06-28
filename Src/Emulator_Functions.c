
/*******************************************************************
** FILE:
**   	Emulator_Functions
** DESCRIPTION:
** 		This file contains the helper functions exclusive to running
** 		emulation mode. These functions are implemented separatly
** 		(or not at all) in real-time mode.
********************************************************************/


/*******************************************************************
** Includes ********************************************************
********************************************************************/

#ifndef COMMON_CONFIG_H
	#include "../Include/Common_Config.h"
#endif
#if EXE_MODE==1 /* Emulator Mode */
	/* In emulatiom mode, "Emulator_Protos" is needed to
	** use funcitons in other files.
	** NOTE: This header should contain the function
	** 			 prototypes for all execution functions */
	#include "../Include/Emulator_Protos.h"
#endif  /* End Emulator Mode */

/* Only link if in emulation mode */
#if EXE_MODE==1

/*******************************************************************
** Functions *******************************************************
********************************************************************/

/*************************************************
** FUNCTION: Init_Emulator
** VARIABLES:
**		[I ]	CONTROL_TYPE		*p_control
** RETURN:
**		NONE
** DESCRIPTION:
**		This function initialized the control struct to
**		to handle emulation mode execution.
*/
void Init_Emulator( CONTROL_TYPE *p_control )
{
  LOG_INFO("> Initializing Emulator\n");

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
**		[I ]	CONTROL_TYPE			*p_control
**		[IO]	SENSOR_STATE_TYPE	*p_sensor_state
** RETURN:
**		NONE
** DESCRIPTION:
*/
void Read_Sensors( CONTROL_TYPE				*p_control,
								 	 SENSOR_STATE_TYPE	*p_sensor_state )
{


  float temp;
  //int   count;

	float gyro_mag;
	float accel_mag;


  //count = fread(&temp,sizeof(float),1,p_control->emu_data.InputFID);
  //if ( count != 1 )
  //{
  //  fprintf(stdout,"Emulator Exit\n");
  //  p_control->emu_data.flag = 0;
  //  return;
  //}
  //Format:1
  // Data Collection: 1
  // Format:
  //   14x[float32] 14x[float32] ....
  //   [Timestamp][accel1][accel2][accel3][gyro1][gyro2][gyro3][yaw][pitch][roll][aest_ave1][aest_ave2][vest_ave1][vest_ave2]
  //p_control->emu_data.timestamp = (unsigned long) temp;
  //fread(&p_sensor_state->accel[0],sizeof(float),3,p_control->emu_data.InputFID);
  //fread(&p_sensor_state->gyro[0],sizeof(float),3,p_control->emu_data.InputFID);
  //fseek(p_control->emu_data.InputFID,sizeof(float)*7,SEEK_CUR);

  //count = fread(&temp,sizeof(float),1,p_control->emu_data.InputFID);
  //if ( count != 1 )
  //{
  //  fprintf(stdout,"Emulator Exit\n");
  //  p_control->emu_data.flag = 0;
  //  return;
  //}
  // Format: 2
  // Data Collection: 2
  //   10x[float32] 10x[float32] ...
  //   [1]        [2]     [3]     [4]     [5]    [6]    [7]    [8]  [9]    [10]
  //   [Timestamp][accel1][accel2][accel3][gyro1][gyro2][gyro3][yaw][pitch][roll]
  //p_control->emu_data.timestamp = (unsigned long) temp;
  //fread(&p_sensor_state->accel[0],sizeof(float),3,p_control->emu_data.InputFID);
  //fread(&p_sensor_state->gyro[0],sizeof(float),3,p_control->emu_data.InputFID);
  //fseek(p_control->emu_data.InputFID,sizeof(float)*3,SEEK_CUR);

  /* Format : Meta V1
  ** This is the first meta enabled data storage type
  ** In real-time, the meta data would be incomplete.
  ** The packet would be re-processed post collection to
  ** fill out missing meta information. */
  if( p_control->SampleNumber==0 )
  {
    /* On first sample, read meta header */
    DATA_READ( &p_control->meta_packet,sizeof(META_PACKET_TYPE), 1, p_control->emu_data.InputFID );
  }
  DATA_READ( &temp, sizeof(float), 1, p_control->emu_data.InputFID);
  p_control->emu_data.timestamp = (unsigned long)temp;
  DATA_READ( &p_sensor_state->accel[0], sizeof(float), 3, p_control->emu_data.InputFID );
  DATA_READ( &p_sensor_state->gyro[0],  sizeof(float), 3, p_control->emu_data.InputFID );
  DATA_READ( &temp,  sizeof(float), 1, p_control->emu_data.InputFID ); /* yaw */
  DATA_READ( &temp,  sizeof(float), 1, p_control->emu_data.InputFID ); /* pitch */
  DATA_READ( &temp,  sizeof(float), 1, p_control->emu_data.InputFID ); /* roll */

//  LOG_INFO( "SampleNumber: %d",                   p_control->SampleNumber );
//  LOG_INFO( "timestamp: %i",                      p_control->emu_data.timestamp);
//  LOG_INFO( "accel[0]: %f",                       p_sensor_state->accel[0]);
//  LOG_INFO( "gyro[0]: %f",                        p_sensor_state->gyro[0]);
//  LOG_INFO( "version_id: %f",                     p_control->meta_packet.version_id  );
//  LOG_INFO( "header_length: %f",                  p_control->meta_packet.header_length  );
//  LOG_INFO( "collection_id: %f",                  p_control->meta_packet.collection_id  );
//  LOG_INFO( "collection_date: %f",                p_control->meta_packet.collection_date  );
//  LOG_INFO( "platform_used: %f",                  p_control->meta_packet.platform_used  );
//  LOG_INFO( "data_quality: %f",                   p_control->meta_packet.data_quality  );
//  LOG_INFO( "collection_subject_id: %f",          p_control->meta_packet.collection_subject_id  );
//  LOG_INFO( "imu_position: %f",                   p_control->meta_packet.imu_position  );
//  LOG_INFO( "collection_env: %f",                 p_control->meta_packet.collection_env  );
//  LOG_INFO( "multiple_speed_flag: %f",            p_control->meta_packet.multiple_speed_flag  );
//  LOG_INFO( "speed: %f",                          p_control->meta_packet.speed  );
//  LOG_INFO( "mult_incline_flag: %f",              p_control->meta_packet.mult_incline_flag  );
//  LOG_INFO( "incline_pct: %f",                    p_control->meta_packet.incline_pct  );
//  LOG_INFO( "time_scale: %f",                     p_control->meta_packet.time_scale  );
//  LOG_INFO( "sample_rate_flag: %f",               p_control->meta_packet.sample_rate_flag  );
//  LOG_INFO( "sample_rate_ave: %f",                p_control->meta_packet.sample_rate_ave  );
//  LOG_INFO( "sample_rate_std: %f",                p_control->meta_packet.sample_rate_std  );
//  LOG_INFO( "number_of_elements_per_sample: %f",  p_control->meta_packet.number_of_elements_per_sample  );
//  LOG_INFO( "orientation_PO: %f",                 p_control->meta_packet.orientation_PO  );
//  LOG_INFO( "orientation_PRC: %f",                p_control->meta_packet.orientation_PRC  );
//  LOG_INFO( "orientation_RRC: %f",                p_control->meta_packet.orientation_RRC  );
//  LOG_INFO( "orientation_ZR: %f",                 p_control->meta_packet.orientation_ZR  );

  /* Increment sample number count */
  p_control->SampleNumber++;

  /* TO DO : Move these computations to math */

  /* Update gyro stats */
  gyro_mag  = Vector_Magnitude ( &p_sensor_state->gyro[0] );
  if( p_control->SampleNumber == 1 )
  {
  	p_sensor_state->gyro_Ave = gyro_mag;
  	p_sensor_state->gyro_mAve = gyro_mag;
	  p_sensor_state->gyro_M2   = gyro_mag;
	  p_sensor_state->gyro_sVar = 0.0;
	  p_sensor_state->gyro_pVar = 0.0;
	}
	temp = p_sensor_state->gyro_Ave;
	p_sensor_state->gyro_Ave  = Rolling_Mean( p_control->SampleNumber, p_sensor_state->gyro_Ave, gyro_mag );
	p_sensor_state->gyro_mAve = Windowed_Mean( p_sensor_state->gyro_mAve, gyro_mag, p_control->SampleNumber, 0.005  );
	p_sensor_state->gyro_M2   = Rolling_SumOfSquares( temp, p_sensor_state->gyro_mAve, gyro_mag, p_sensor_state->gyro_M2 );
	p_sensor_state->gyro_sVar = Rolling_Sample_Variance( p_control->SampleNumber, p_sensor_state->gyro_M2 );
	p_sensor_state->gyro_pVar = Rolling_Population_Variance( p_control->SampleNumber, p_sensor_state->gyro_M2 );

  /* Update accel stats */
  accel_mag = Vector_Magnitude ( &p_sensor_state->accel[0] );
  if( p_control->SampleNumber == 1 )
  {
  	p_sensor_state->accel_Ave  = accel_mag;
  	p_sensor_state->accel_mAve = accel_mag;
	  p_sensor_state->accel_M2   = accel_mag;
	  p_sensor_state->accel_sVar = 0.0;
	  p_sensor_state->accel_pVar = 0.0;
  }
	temp = p_sensor_state->accel_Ave;
	p_sensor_state->accel_Ave  = Rolling_Mean( p_control->SampleNumber, p_sensor_state->accel_Ave, accel_mag );
	p_sensor_state->accel_mAve = Windowed_Mean( p_sensor_state->accel_mAve, accel_mag, p_control->SampleNumber, 0.005  );
	p_sensor_state->accel_M2   = Rolling_SumOfSquares( temp, p_sensor_state->accel_mAve, accel_mag, p_sensor_state->accel_M2 );
	p_sensor_state->accel_sVar = Rolling_Sample_Variance( p_control->SampleNumber, p_sensor_state->accel_M2 );
	p_sensor_state->accel_pVar = Rolling_Population_Variance( p_control->SampleNumber, p_sensor_state->accel_M2 );

} /* End Read_Sensors */


/*************************************************
** FUNCTION: delay
** VARIABLES:
**		[I ]	unsigned int mseconds
** RETURN:
**		NONE
** DESCRIPTION:
**		This function emulates the delay function
**		in Arduino.
**		This function is unnecessary and should't
**		be called.
*/
void delay(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}


///*************************************************
//** FUNCTION: LogInfoToFile
//** VARIABLES:
//**    [I ]  CONTROL_TYPE         *p_control
//**    [I ]  OUTPUT_LOG_FILE_TYPE *log_file,
//**    [I ]  char*                 message
//** RETURN:
//**    void
//** DESCRIPTION:
//**    This is a helper function.
//**    It is used for logging to a local txt file
//**    This function will add data to the output buffers
//**    and (when the buffer has reached the defined length)
//**    write the buffer to the appropriate output file.
//**    NOTE: This function pairs with the LOG_INFO macro.
//**          For the LOG_DATA pair, see LogDataToFile.
//**    NOTE: This function replaces the IMU_MODE function
//**          LogInfoToFile.
//*/
//void LogInfoToFile( CONTROL_TYPE         *p_control,
//                    OUTPUT_LOG_FILE_TYPE *log_file,
//                    char*                 msg  )
//{
//  long int size_bytes;
//
//  /* Add message to output buffer */
//  log_file->LogBufferLen += strlen( msg );
//  strcat( log_file->LogBuffer, msg );
//  strcat( log_file->LogBuffer, "\n" );
//
//  /* If buffer has reached designated size ...*/
//  if( log_file->LogBufferLen>MAX_LOG_BUFFER_SIZE )
//  {
//    /* NOTE : File is kept open during execution
//    **        First open is in common_init */
//
//    /* Get current file size */
//    //size_bytes = ftell( log_file->LogFile_fh );
//    size_bytes = FILE_SIZE_BYTES(log_file->LogFile_fh);
//
//    /* If the filesize is larger than the designated
//    ** max, close file and open next index */
//    if( size_bytes>MAX_OUTPUT_FILE_SIZE )
//    {
//      //fclose( log_file->LogFile_fh );
//      FILE_CLOSE( log_file->LogFile_fh );
//      GetNextLogFileName( p_control, log_file );
//
//      //log_file->LogFile_fh = fopen( log_file->LogFileName, "w" );
//      log_file->LogFile_fh = FILE_OPEN_WRITE( log_file->LogFileName );
//    }
//
//    /* Print to file */
//    //fprintf( log_file->LogFile_fh, log_file->LogBuffer );
//    //fflush( log_file->LogFile_fh );
//    FILE_PRINT_TO_FILE( log_file->LogFile_fh, log_file->LogBuffer );
//    FILE_FLUSH(_fh);
//
//    log_file->LogBuffer[0] = '\0';
//    log_file->LogBufferLen = 0;
//  }
//} /* End LogInfoToFile() */

///*************************************************
//** FUNCTION: GetNextLogFileName
//** VARIABLES:
//**    [I ]  CONTROL_TYPE      *p_control
//** RETURN:
//**    void
//** DESCRIPTION:
//**    This is a helper function.
//**    It is used for logging to an SD card.
//**    This function creates a filename which does not
//**    exist on the card, to which we will log our data.
//*/
//void GetNextLogFileName( CONTROL_TYPE          *p_control,
//                         OUTPUT_LOG_FILE_TYPE  *log_file )
//{
//  int  i;
//
//  for( i=log_file->LogFileIdx; i<LOG_FILE_MAX_IDX; i++ )
//  {
//    /* Construct a file with PREFIX[Index].SUFFIX */
//    sprintf( log_file->LogFileName, "%s%i.%s", log_file->file_prefix, i, log_file->file_suffix );
//    LOG_INFO( " > Trying File %s", log_file->LogFileName );
//
//    /* If the file name doesn't exist, return it */
//    //if( fopen(log_file->LogFileName, "r") == NULL )
//    if( ! FILE_EXISTS_FLAG(log_file->LogFileName) )
//    {
//      LOG_INFO( " > File %s Available", log_file->LogFileName );
//      log_file->LogFileIdx = i + 1;
//      break;
//    }
//  }
//} /* End GetNextLogFileName() */

#endif /* End EXE_MODE (Emulation mode) */
