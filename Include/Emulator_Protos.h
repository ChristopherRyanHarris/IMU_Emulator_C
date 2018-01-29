
/*******************************************************************
** FILE:
**   	Emulator_Protos.h
** DESCRIPTION:
** 		This file contains all header information specific to the
** 		the IMU emulator. The IMU emulator uses previously collected
** 		data. This allows us to test the preformance of the algorithm
********************************************************************/
#ifndef EMULATOR_PROTOS_H
#define EMULATOR_PROTOS_H


/*******************************************************************
** Includes ********************************************************
********************************************************************/

#ifndef COMMON_CONFIG_H
	#include "../Include/Common_Config.h"
#endif
#if EXE_MODE==1 /* Emulator mode */
	#include <math.h>
	#include <string.h>

	#include "../Include/Emulator_Config.h"
	#include "../Include/Calibration_Config.h"
	#include "../Include/DSP_Config.h"
	#include "../Include/DCM_Config.h"
	#include "../Include/GaPA_Config.h"
	#include "../Include/WISE_Config.h"
	#include "../Include/Communication_Config.h"
#endif /* End Emulator mode */

/*******************************************************************
** Prototypes ******************************************************
********************************************************************/


/*************************************************
** Emulator Functions */

void Init_Emulator(
			CONTROL_TYPE			*p_control );

void Read_Sensors(
			CONTROL_TYPE			*p_control,
			SENSOR_STATE_TYPE	*p_sensor_state );

void delay(unsigned int mseconds);


/*************************************************
** COMMON_FUNCTIONS */

void Common_Init(
			CONTROL_TYPE 			*p_control );

void Update_Time(
			CONTROL_TYPE 			*p_control );


/*************************************************
** DSP_Functions */

void DSP_Filter_Init(
			CONTROL_TYPE			*p_control,
			DSP_STATE_TYPE		*p_dsp_state );

void DSP_Update(
			CONTROL_TYPE			*p_control,
			DSP_STATE_TYPE		*p_dsp_state,
			SENSOR_STATE_TYPE *p_sensor_state );

void DSP_Shift (
			CONTROL_TYPE			*p_control,
			DSP_STATE_TYPE		*p_dsp_state );
void IIR_Filter(
			CONTROL_TYPE			*p_control,
			DSP_STATE_TYPE		*p_dsp_state,
			SENSOR_STATE_TYPE *p_sensor_state );
void FIR_Filter(
			CONTROL_TYPE			*p_control,
			DSP_STATE_TYPE		*p_dsp_state,
			SENSOR_STATE_TYPE *p_sensor_state );


/*************************************************
** CAL_Functions */

void Calibration_Init(
			CONTROL_TYPE			*p_control,
			CALIBRATION_TYPE 	*p_calibration );

void Calibrate(
			CONTROL_TYPE			*p_control,
			CALIBRATION_TYPE	*p_calibration,
			SENSOR_STATE_TYPE	*p_sensor_state );


/*************************************************
** DCM_Functions */

void DCM_Init(
			CONTROL_TYPE			*p_control,
			DCM_STATE_TYPE		*p_dcm_state,
			SENSOR_STATE_TYPE	*p_sensor_state );

void Init_Rotation_Matrix(
			CONTROL_TYPE			*p_control,
			DCM_STATE_TYPE		*p_dcm_state,
			SENSOR_STATE_TYPE	*p_sensor_state );

void DCM_Filter(
			CONTROL_TYPE			*p_control,
			DCM_STATE_TYPE		*p_dcm_state,
			SENSOR_STATE_TYPE	*p_sensor_state );

void Reset_Sensor_Fusion(
			CONTROL_TYPE			*p_control,
			DCM_STATE_TYPE		*p_dcm_state,
			SENSOR_STATE_TYPE	*p_sensor_state );

void Set_Sensor_Fusion(
			CONTROL_TYPE			*p_control,
			SENSOR_STATE_TYPE	*p_sensor_state );


/*************************************************
** GaPA_Functions */

void GaPA_Init(
			CONTROL_TYPE			*p_control,
			GAPA_STATE_TYPE		*p_gapa_state );

void GaPA_Reset(
			CONTROL_TYPE			*p_control,
			GAPA_STATE_TYPE		*p_gapa_state );

void GaPA_Update(
			CONTROL_TYPE			*p_control,
			SENSOR_STATE_TYPE *p_sensor_state,
			GAPA_STATE_TYPE		*p_gapa_state );

void TrackPhiVariables(
			GAPA_STATE_TYPE		*p_gapa_state );

void calc_SftPrmLeft(
			float	*GAMMA,
			float	PHI_max,
			float PHI_min );

void calc_SftPrmRight(
			float	*gamma,
			float	phi_max,
			float phi_min );

void calc_ScaleFactor(
			float *z,
			float phi_max,
			float phi_min,
			float PHI_max,
			float PHI_min );

void calc_PhaseAngle(
			float* nu,
			float z,
			float PHI,
			float GAMMA,
			float phi,
			float gamma );


/*************************************************
** WISE_Functions */

void WISE_Init(
			CONTROL_TYPE			*p_control,
			SENSOR_STATE_TYPE	*p_sensor_state,
			WISE_STATE_TYPE		*p_wise_state );

void WISE_Reset(
			CONTROL_TYPE			*p_control,
			WISE_STATE_TYPE		*p_wise_state );

void WISE_Update(
			CONTROL_TYPE			*p_control,
			SENSOR_STATE_TYPE	*p_sensor_state,
			WISE_STATE_TYPE		*p_wise_state );

void Map_Accel_2D(
			CONTROL_TYPE			*p_control,
			SENSOR_STATE_TYPE	*p_sensor_state,
			WISE_STATE_TYPE		*p_wise_state );

void Integrate_Accel_2D(
			CONTROL_TYPE			*p_control,
			SENSOR_STATE_TYPE	*p_sensor_state,
			WISE_STATE_TYPE		*p_wise_state );

void Adjust_Velocity(
			CONTROL_TYPE			*p_control,
			SENSOR_STATE_TYPE	*p_sensor_state,
			WISE_STATE_TYPE		*p_wise_state );

void Adjust_Incline(
			CONTROL_TYPE			*p_control,
			SENSOR_STATE_TYPE	*p_sensor_state,
			WISE_STATE_TYPE		*p_wise_state );

void Estimate_Error(
			CONTROL_TYPE			*p_control,
			SENSOR_STATE_TYPE	*p_sensor_state,
			WISE_STATE_TYPE		*p_wise_state );


/*************************************************
** LOGGING_Functions */

void Debug_LogOut(
			CONTROL_TYPE			*p_control,
			SENSOR_STATE_TYPE	*p_sensor_state,
			WISE_STATE_TYPE		*p_wise_state );

void Cal_LogOut(
      CONTROL_TYPE			 *p_control,
			SENSOR_STATE_TYPE  *p_sensor_state,
			CALIBRATION_TYPE	 *p_calibration );

void FltToStr(
      float value,
      int   precision,
      char  *StrBuffer );


/*************************************************
** Communication_Functions */

void f_SendData( int nBytesIn );

void f_SendPacket( COMMUNICATION_PACKET_TYPE Response );

void f_WriteIToPacket(
			uint8_t 			*Packet,
			uint16_t 			InputBuffer );

void f_WriteFToPacket_u16(
			unsigned char *Packet,
			float 				Input );

void f_WriteFToPacket_s32(
			unsigned char *Packet,
			float 				Input );

void f_Handshake(
			CONTROL_TYPE 	*p_control );

uint8_t f_CheckSum(
			unsigned char *p_Buffer,
			uint16_t 			nBytes );


/*************************************************
** Math */

float Vector_Dot_Product(
			const float v1[3],
			const float v2[3] );

void Vector_Cross_Product(
			const float v1[3],
			const float v2[3],
			float 			out[3] );

void Vector_Scale(
			const float v[3],
			float 			scale,
			float 			out[3] );

void Vector_Add(
			const float v1[3],
			const float v2[3],
			float 			out[3] );

void Matrix_Multiply(
			const float m1[3][3],
			const float m2[3][3],
			float 			out[3][3] );

void Matrix_Vector_Multiply(
			const float m[3][3],
			const float v[3],
			float 			out[3] );

float Rolling_Mean(
			const int n,
			float 		m,
			float 		x );

float Rolling_Variance(
			const float m_prev,
			const float m,
			float 			x,
			float 			S );

float Windowed_Mean(
			float m,
			float x,
			int 	n,
			float a );

float f_asin( float x );

float f_atan2(
			float y,
			float x );

void calc_circle_center(
			float p1[2],
			float p2[2],
			float p3[2],
			float xcyc[2] );


#endif /* End EMULATOR_PROTOS_H */








