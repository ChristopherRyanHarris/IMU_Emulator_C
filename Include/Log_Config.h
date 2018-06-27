
/*******************************************************************
** FILE:
**    Log_Config.h
** DESCRIPTION:
**    The header contains all the logging functions, regardless
**    of platform. Note that the "communiction" functions typically
**    refer to the binary messaging type functions.
********************************************************************/

#ifndef LOGGING_CONFIG_H
#define LOGGING_CONFIG_H





/******************************************************************
** Logging Parameters
*******************************************************************/

#define ENABLE_SD_LOGGING 0
#define ENABLE_C_FILE_LOGGING 0 /* EXE=1 Mode */

/* Serial Port Configuration */
//#define UART_PORT_BAUD 115200
#define UART_PORT_BAUD 250000

//#define SERIAL_PORT_BAUD 9600
#define SERIAL_PORT_BAUD 250000

/* Data LOG period (us) */
#define DATA_LOG_RATE 1

/* The LED can be used for external debugging */
#define LED_BLINK_RATE 300

#define MAX_LINE_LEN 500
#define MAX_BUFFER_LEN ONE_KILOBYTE

/* Data/Meta log packet version */
#define DATA_PACKET_VERSION 1


/******************************************************************
** SD/File Logging Parameters
*******************************************************************/
#define SD_PIN 38

#define LOG_FILE_MAX_IDX 999  /* Max number of "log.txt" files */

#define LOG_INFO_FILE_PREFIX "log_info_" /* Prefix name for log info files */
#define LOG_INFO_FILE_SUFFIX "txt"       /* Suffix name for log info files */

#define LOG_DATA_FILE_PREFIX "log_data_" /* Prefix name for log data files */
#define LOG_DATA_FILE_SUFFIX "bin"       /* Suffix name for log data files */

#define MAX_LOG_BUFFER_SIZE TWO_KILOBYTE
#define MAX_OUTPUT_FILE_SIZE ONE_GIGABYTE /* 1 GB max file size */

#if( EXE_MODE==0 ) /* 0 : IMU Mode */
  #define FILE_TYPE File
#else
  #define FILE_TYPE FILE*
#endif

/******************************************************************
** Logging Types
*******************************************************************/

/* Define the meta packet type */
typedef struct
{
	float version_id;
	float header_length;
	float collection_id;
	float collection_date;
	float platform_used;

	float data_quality;
	float collection_subject_id;
	float imu_position;
	float collection_env;
	float multiple_speed_flag;
	float speed;
	float mult_incline_flag;
	float incline_pct;

	float time_scale;
	float sample_rate_flag;
	float sample_rate_ave;
	float sample_rate_std;
	float number_of_samples;
	float number_of_elements_per_sample;
	float orientation_PO;
	float orientation_PRC;
	float orientation_RRC;
	float orientation_ZR;
}	LOG_META_PACKET_V1_TYPE;

/* Define meta packet type to use */
#if DATA_PACKET_VERSION==1
	#define META_PACKET_TYPE LOG_META_PACKET_V1_TYPE
#else
	#error "Meta Packet Is not Defined!"
#endif


typedef struct
{
	/* Common Log controls */
	FILE_TYPE LogFile_fh;
	int       LogFileIdx;
  char      LogFileName[50];
  int       LogBufferLen;
  char      LogBuffer[2048];

  char      file_suffix[50];
  char      file_prefix[50];

  /* SD controls */

} OUTPUT_LOG_FILE_TYPE;

/******************************************************************
** Logging Macros
*******************************************************************/

#define LOG_INFO_FAIL_STRING "<ERROR::FATAL FAIL>"
#define LOG_INFO_WARNING_STRING "<WARNING>"

#define DATA_READ_ASSERT( _n, _nmemb ) \
do{	\
	if( (_n)!=(_nmemb) ) \
	{ \
		LOG_INFO_FAIL("Read Failed! Expected:%d Read:%d",_nmemb,_n ); \
	} \
}while(0)

#define DATA_WRITE_ASSERT( _n, _nmemb ) \
do{	\
	if( (_n)!=(_nmemb) ) \
	{ \
		LOG_INFO_FAIL("Write Failed! Expected:%d Wrote:%d",_nmemb,_n ); \
	} \
}while(0)

#define DATA_READ( _addr, _sizeBytes, _nmemb, _fh ) \
do{ \
	int _c = fread( _addr, _sizeBytes, _nmemb, _fh ); \
  DATA_READ_ASSERT( _c, _nmemb ); \
}while(0)




#if EXE_MODE==0 /* 0 : IMU Mode */

  /* The debug port is simply the serial terminal */
  #define UART_PORT SERIAL_PORT_USBVIRTUAL

  /* Serial I/O */
  #define SERIAL_PORT  SERIAL_PORT_USBVIRTUAL
  #define SERIAL_PRINT SERIAL_PORT.print
  #define SERIAL_WRITE SERIAL_PORT.write
  #define SERIAL_READ  SERIAL_PORT.read()
  #define SERIAL_AVAILABLE SERIAL_PORT.available()

  /* Print [func,line] */
  #define LOG_TAG_UART \
  do{ \
    char _buffer_log_tag_db[MAX_LINE_LEN]; \
    sprintf(_buffer_log_tag_db,"[%s:%d] ",__FUNCTION__,__LINE__); \
    UART_PORT.print(_buffer_log_tag_db); \
  }while(0)

  /* Print [long_func, line] */
  #define LOG_TAG_UART_LONG \
  do{ \
    char _buffer_log_tag_lg_db[MAX_LINE_LEN]; \
    sprintf(_buffer_log_tag_lg_db,"[%s:%d] ",__PRETTY_FUNCTION__,__LINE__); \
    UART_PORT.print(_buffer_log_tag_lg_db); \
  }while(0)

  /* In the SEN-14001 platform, we have the
  ** ability to log to an sd card (if present) */
  #if( ENABLE_SD_LOGGING==TRUE ) /* Using SD Card */

    /* Print message with forced newline */
    #define LOG_INFO(...) \
    do{ \
      char _buffer_dat[MAX_LINE_LEN]; \
      char _buffer_log_pnt_db[MAX_LINE_LEN]; \
      sprintf(_buffer_log_pnt_db,__VA_ARGS__); \
      sprintf( _buffer_dat, "[%s:%d] %s", __FUNCTION__,__LINE__, _buffer_log_pnt_db ); \
      LogInfoToSDFile( p_control, &p_control->log_info_file, _buffer_dat ); \
    }while(0)

    /* Print failure message with forced newline */
    #define LOG_INFO_FAIL(...) \
    do{ \
      char _buffer_dat[MAX_LINE_LEN]; \
      char _buffer_log_pnt_db[MAX_LINE_LEN]; \
      sprintf(_buffer_log_pnt_db,__VA_ARGS__); \
      sprintf( _buffer_dat, "[%s:%d] %s %s", __FUNCTION__,__LINE__, LOG_INFO_FAIL_STRING, _buffer_log_pnt_db ); \
      LogInfoToSDFile( p_control, &p_control->log_info_file, _buffer_dat ); \
    }while(0)

    /* Print warning message with forced newline */
    #define LOG_INFO_WARNING(...) \
    do{ \
      char _buffer_dat[MAX_LINE_LEN]; \
      char _buffer_log_pnt_db[MAX_LINE_LEN]; \
      sprintf(_buffer_log_pnt_db,__VA_ARGS__); \
      sprintf( _buffer_dat, "[%s:%d] %s %s", __FUNCTION__,__LINE__, LOG_INFO_WARNING_STRING, _buffer_log_pnt_db ); \
      LogInfoToSDFile( p_control, &p_control->log_info_file, _buffer_dat ); \
    }while(0)

    /* Record Binary Data to SD file */
    #define LOG_DATA( _dat, _size, _count ) \
    do{ \
      LogDataToSDFile( p_control, &p_control->log_info_file, _buffer_dat ); \
    }while(0)

  #else /* SD_LOGGING mode not enabled */

    /* If SD recording is disabled,
    ** we default to logging to UART */
    #define LOG_DATA LOG_INFO

    /* Print message with forced newline */
    #define LOG_INFO(...) \
    do{ \
      char _buffer[MAX_LINE_LEN]; \
      sprintf(_buffer,__VA_ARGS__); \
      LOG_TAG_UART; \
      UART_PORT.println(_buffer); \
      UART_PORT.flush(); \
    }while(0)

    /* Print failure message with forced newline */
    #define LOG_INFO_FAIL(...) \
    do{ \
      char _buffer[MAX_LINE_LEN]; \
      sprintf(_buffer,LOG_INFO_FAIL_STRING); \
      sprintf(_buffer," "); \
      sprintf(_buffer,__VA_ARGS__); \
      LOG_TAG_UART; \
      UART_PORT.println(_buffer); \
      UART_PORT.flush(); \
    }while(0)

    /* Print warning message with forced newline */
    #define LOG_INFO_WARNING(...) \
    do{ \
      char _buffer[MAX_LINE_LEN]; \
      sprintf(_buffer,LOG_INFO_WARNING_STRING); \
      sprintf(_buffer," "); \
      sprintf(_buffer,__VA_ARGS__); \
      LOG_TAG_UART; \
      UART_PORT.println(_buffer); \
      UART_PORT.flush(); \
    }while(0)

  #endif /* End SD_LOGGING Block */

#else /* EXE_MODE==1 , c executable code */


  /* Mock Serial I/O */
  #define SERIAL_PRINT(...) /* Do Nothing */
  #define SERIAL_WRITE(...) /* Do Nothing */
  #define SERIAL_READ 0  /* Do Nothing */
  #define SERIAL_AVAILABLE 0 /* Do Nothing */

  ///* Record Binary Data */
  //#define LOG_DATA(...) \
  //do{ \
  //  char _buffer_dat[MAX_LINE_LEN]; \
  //  char _buffer_log_pnt_db[MAX_LINE_LEN]; \
  //  sprintf(_buffer_log_pnt_db,__VA_ARGS__); \
  //  sprintf( _buffer_dat, "[%s:%d] %s", __FUNCTION__,__LINE__, _buffer_log_pnt_db ); \
  //  LogDataToFile( p_control, &p_control->log_info_file, _buffer_dat ); \
  //}while(0)

  #if( ENABLE_C_FILE_LOGGING==1 )

    /* TO DO : Add LOG_DATA function for binary file in EXE_MODE==1 (C) */
    #define LOG_DATA LOG_INFO

    /* Print message with forced newline */
    #define LOG_INFO(...) \
    do{ \
      char _buffer_dat[MAX_LINE_LEN]; \
      char _buffer_log_pnt_db[MAX_LINE_LEN]; \
      sprintf(_buffer_log_pnt_db,__VA_ARGS__); \
      sprintf( _buffer_dat, "[%s:%d] %s\n", __FUNCTION__,__LINE__, _buffer_log_pnt_db ); \
      LogInfoToFile( p_control, &p_control->log_info_file, _buffer_dat ); \
    }while(0)

    /* Print failure message with forced newline */
    #define LOG_INFO_FAIL(...) \
    do{ \
      char _buffer_dat[MAX_LINE_LEN]; \
      char _buffer_log_pnt_db[MAX_LINE_LEN]; \
      sprintf(_buffer_log_pnt_db,__VA_ARGS__); \
      sprintf( _buffer_dat, "[%s:%d] %s %s\n", __FUNCTION__,__LINE__, LOG_INFO_FAIL_STRING, _buffer_log_pnt_db ); \
      LogInfoToFile( p_control, &p_control->log_info_file, _buffer_dat ); \
    }while(0)

    /* Print warning message with forced newline */
    #define LOG_INFO_WARNING(...) \
    do{ \
      char _buffer_dat[MAX_LINE_LEN]; \
      char _buffer_log_pnt_db[MAX_LINE_LEN]; \
      sprintf(_buffer_log_pnt_db,__VA_ARGS__); \
      sprintf( _buffer_dat, "[%s:%d] %s %s\n", __FUNCTION__,__LINE__, LOG_INFO_WARNING_STRING, _buffer_log_pnt_db ); \
      LogInfoToFile( p_control, &p_control->log_info_file, _buffer_dat ); \
    }while(0)

  #else /* Log to stdout */

    #define LOG_DATA LOG_INFO

    /* Print message with forced newline */
    #define LOG_INFO(...) \
    do{ \
      char _buffer_log_pnt_db[MAX_LINE_LEN]; \
      sprintf(_buffer_log_pnt_db,__VA_ARGS__); \
      fprintf( stdout, "[%s:%d] %s\n", __FUNCTION__,__LINE__, _buffer_log_pnt_db ); \
    }while(0)

    /* Print failure message with forced newline */
    #define LOG_INFO_FAIL(...) \
    do{ \
      char _buffer_log_pnt_db[MAX_LINE_LEN]; \
      sprintf(_buffer_log_pnt_db,__VA_ARGS__); \
      fprintf( stdout, "[%s:%d] %s %s\n", __FUNCTION__,__LINE__, LOG_INFO_FAIL_STRING, _buffer_log_pnt_db ); \
    }while(0)

    /* Print warning message with forced newline */
    #define LOG_INFO_WARNING(...) \
    do{ \
      char _buffer_log_pnt_db[MAX_LINE_LEN]; \
      sprintf(_buffer_log_pnt_db,__VA_ARGS__); \
      fprintf( stdout, "[%s:%d] %s %s\n", __FUNCTION__,__LINE__, LOG_INFO_WARNING_STRING, _buffer_log_pnt_db ); \
    }while(0)

  #endif /* End if( ENABLE_C_FILE_LOGGING==1 ) */

#endif /* End EXE_MODE==1 , c executable code */



#endif /* End LOG_CONFIG_H */



