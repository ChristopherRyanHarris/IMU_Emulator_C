
/*******************************************************************
** FILE: 
**   	IMU9250_Functions
** DESCRIPTION:
** 		This file contains some MPU 9250 (HW specific)
** 		functions. Specifically, for initializing and 
** 		reading the sensor registeres
********************************************************************/


#ifdef _IMU9250_


/*******************************************************************
** Includes ********************************************************
********************************************************************/

/*******************************************************************
** Functions *******************************************************
********************************************************************/


/*************************************************
** FUNCTION: Init_IMU
** VARIABLES:
**		[I ]	CONTROL_TYPE 			*p_control
** RETURN:
**		BOOL	1:Successful initialization
**					0:Failure
** DESCRIPTION: 
** 		This function set the IMU parameters
** 		This includes things like the SR and the 
** 		internal LPF corner. 
*/
bool Init_IMU( CONTROL_TYPE	*p_control )
{
	unsigned char activate_sensors = 0;
	
	LOG_PRINTLN("> Initializing IMU9250");
	
  /* Set up MPU-9250 interrupt input (active-low) */
  pinMode(MPU9250_INT_PIN, INPUT_PULLUP);
  
  /* imu.begin() should return 0 on success. Will initialize
  ** I2C bus, and reset MPU-9250 to defaults */
  if (imu.begin() != INV_SUCCESS) { return FALSE; }

  /* Initiate accel and gyro sensors only */
  #if 
  /* Turn on or off MPU-9250 sensors. Any of the following defines can be combined: \
  ** 		INV_XYZ_GYRO, INV_X_GYRO, INV_Y_GYRO, or INV_Z_GYRO, 
  **		INV_XYZ_ACCEL, or INV_XYZ_COMPASS
	** Input: Combination of enabled sensors. 
	** Unless specified a sensor will be disabled.
	** Output: INV_SUCCESS (0) on success, otherwise error */
	#if ACCEL_ON==1
		activate_sensors = activate_sensors | INV_XYZ_ACCEL;
	#endif
	#if GYRO_ON==1
		activate_sensors = activate_sensors | INV_XYZ_GYRO;
	#endif
	#if MAGN_ON==1
		activate_sensors = activate_sensors | INV_XYZ_COMPASS;
	#endif
  imu.setSensors( activate_sensors );

  /* Configure sensors: */
  imu.setGyroFSR( IMU_GYRO_FSR );
  imu.setAccelFSR( IMU_ACCEL_FSR );
  imu.setLPF( IMU_AG_LPF );
  imu.setSampleRate( IMU_AG_SAMPLE_RATE );
  
  return TRUE;
} /* End Init_IMU */

/*************************************************
** FUNCTION: Read_Sensors
** VARIABLES:
**		[I ]	CONTROL_TYPE 			*p_control
**		[IO]	SENSOR_STATE_TYPE *p_sensor_state
** RETURN:
**		NONE
** DESCRIPTION: 
** 		This function reads the sensor registers and
** 		assigns them to the global input vectors
*/
void Read_Sensors( CONTROL_TYPE				*p_control,
								   SENSOR_STATE_TYPE	*p_sensor_state )
{
  int i;
  
  /* Read the Accelerometer */
  #if ACCEL_ON==1
  	imu.updateAccel();
  	p_sensor_state->accel[0] = imu.ax;
  	p_sensor_state->accel[1] = imu.ay;
  	p_sensor_state->accel[2] = imu.az;
  #endif 
  
 	/* Read the Gyroscope */
  #if GYRO_ON==1
  	imu.updateGyro();
  	p_sensor_state->gyro[0] = imu.gx;
  	p_sensor_state->gyro[1] = imu.gy;
  	p_sensor_state->gyro[2] = imu.gz;
  #endif 
  
  /* Read the Magnometer */
  #if MAGN_ON==1
  #endif
  
} /* End Read_Sensors */

#endif /* _IMU9250_ */




