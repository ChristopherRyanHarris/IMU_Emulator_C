
/******************************************************************
** FILE: IMU_Common.h
** Header containing IMU definitions which are platform agnostic.
** Definitions in this file should be independent of IMU version.
******************************************************************/


#ifndef COMMON_H
#define COMMON_H

/* 0: IMU
** 1: Emulator */
#define EXE_MODE 1


#if EXE_MODE==1
	/* Emulator mode */
	#include <stdbool.h>
	#include <inttypes.h>
	#include <stdio.h>
	#include "../Include/Emulator_Config.h"
	#include "../Include/DCM_Config.h"

	//#define _IMU10736_ /* Using IMU10736 */
	#define _IMU9250_ /* Using IMU9250 */
#else
	/* Real-time mode */
	//#define _IMU10736_ /* Using IMU10736 */
	#define _IMU9250_ /* Using IMU9250 */
#endif

#define DEBUG 1

/* I/O params */
#define OUTPUT_MODE 4
#define NUM_COM_MODES 6

/* Calibration params  */
#define CALIBRATION_MODE  0 /* 0|1 */
#define CAL_OUTPUT_MODE 0
#define NUM_CALCOM_MODES 2


#define DSP_ON 0
#define GAPA_ON 1
#define WISE_ON 0



/*******************************************************************
** Tyedefs *********************************************************
********************************************************************/

/*
** TYPE: CALIBRATION_TYPE
** This type is used to hold
** data useful for calibration */
typedef struct
{
  float accel_total[3];
  float accel_max[3];
  float accel_min[3];

  float gyro_total[3];
  float gyro_max[3];
  float gyro_min[3];

  float N;
} CALIBRATION_TYPE;

/* TYPE: CALIBRATION_PRMS_TYPE
** This type is used to hold the calibration
** control parameters. */
typedef struct
{
  int output_mode;
} CALIBRATION_PRMS_TYPE;

/*
** TYPE: CONTROL_TYPE
** This type is used to hold all the execution
** parameters. It allows for a more dynamic
** execution. */
typedef struct
{
	int output_mode;

  unsigned long timestamp;
  unsigned long timestamp_old;
  float G_Dt;

  /* Serial communication variables */
  bool     BaudLock;    /* Used to set baud rate */
  uint32_t LastLogTime; /* Sets the UART LOG Rate */

  /* LED state globals */
  bool      LedState; /* Used to set LED state */
  uint32_t  LastBlinkTime; /* Used to set LED state */


  /* If calibration mode,
  ** include calibration struct */
  #if CALIBRATION_MODE==1
  	CALIBRATION_PRMS_TYPE calibration_prms;
  #endif

  /* If in Emulation mode,
  ** include the emulation structure */
  #if EXE_MODE==1
  	EMULATION_TYPE emu_data;
  #endif

} CONTROL_TYPE;





/*
** TYPE: SENSOR_STATE_TYPE
** This type is used to hold the sensor
** variables */
typedef struct
{
  float yaw;
  float pitch;
  float roll;

  float yaw_prev;
  float pitch_prev;
  float roll_prev;

  float mag[3]; /* not used */

  /* Accel x:Fore y:Port z:Zenith */
  float accel[3];
  float gyro[3];

  float gyro_ave[3];
  float gyro_var[3];
  float gyro_std[3];

  float std_time;

} SENSOR_STATE_TYPE;

/*
** TYPE: CONTROL_STATE_TYPE
** This type is used to hold all control
** variables. */
typedef struct
{

} CONTROL_STATE_TYPE;

/*
** TYPE: RESPONSE_TYPE
** Used to store temporary resonse data
** for responding to request from master */
typedef struct
{
  uint16_t       Packet_nBytes;  /* Length of entire packet, minus this variable, in bytes */
  uint16_t       PacketType;     /* Type code of packet */
  uint16_t       Buffer_nBytes;  /* Length of data buffer in bytes (0-50) */
  unsigned char  Buffer[50];     /* Data buffer */
  unsigned char  CheckSum;       /* CheckSum of data buffer only */
} RESPONSE_TYPE;


#endif // IMU_COMMON_H
