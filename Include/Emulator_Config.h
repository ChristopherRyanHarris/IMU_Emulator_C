/******************************************************************
** FILE: Emulator.h
** This file contains all header information specific to the
** the IMU emulator. The IMU emulator uses previously collected
** data. This allows us to test the preformance of the algorithm
******************************************************************/

#ifndef EMULATOR_H
#define EMULATOR_H

/*******************************************************************
** Imcludes ********************************************************
********************************************************************/

#include "IMU10736_Config.h"

#include "DSP_Config.h"
#include "Common_Config.h"
#include "WISE_Config.h"

/*******************************************************************
** Prototypes ******************************************************
********************************************************************/

/* Emulator Functions */
void Init_Emulator( void );
void Read_Sensors( void );

/* COMMON_FUNCTIONS */
void Common_Init( void );


/* CAL_Functions */
void Calibration_Init ( void );
void Calibrate ( void );

/* WISE_Functions */
void WISE_Init ( void );
void WISE_Reset ( void );
void WISE_Update ( void );
void Map_Accel_2D ( void );
void Integrate_Accel_2D ( void );
void Adjust_Velocity ( void );
void Adjust_Incline ( void );
void Estimate_Error ( void );

/* Com_Functions */
void Debug_LogOut( void );
void Cal_LogOut( void );
void f_SendData( int nBytesIn );
void f_SendPacket( RESPONSE_TYPE Response );
void f_WriteIToPacket( uint8_t *Packet, uint16_t InputBuffer );
void f_WriteFToPacket_u16( unsigned char *Packet, float Input );
void f_WriteFToPacket_s32( unsigned char *Packet, float Input );
void f_Handshake( void );
uint8_t f_CheckSum( unsigned char *p_Buffer, uint16_t nBytes );

/* DCM_Functions */
void Update_Time( void );
void Init_Rotation_Matrix(float m[3][3], float yaw, float pitch, float roll);
void DCM_Filter( void );
void Reset_Sensor_Fusion( void );
void Set_Sensor_Fusion( void );

/* DSP_Functions */
void DSP_Filter_Init ( void );
void DSP_Update ( void );
void DSP_Shift ( void );
void IIR_Filter ( void );
void FIR_Filter ( void );

/* HW_Functions */
void Init_Hardware( void );
void Blink_LED( void );

/* Math */
float Vector_Dot_Product( const float v1[3], const float v2[3] );
void Vector_Cross_Product( const float v1[3], const float v2[3], float out[3] );
void Vector_Scale( const float v[3], float scale, float out[3] );
void Vector_Add( const float v1[3], const float v2[3], float out[3] );
void Matrix_Multiply( const float m1[3][3], const float m2[3][3], float out[3][3] );
void Matrix_Vector_Multiply( const float m[3][3], const float v[3], float out[3] );
float Rolling_Mean( const int n, float m, float x );
float Rolling_Std( const float m_prev, const float m, float x, float S );
float f_asin( float x );
float f_atan2( float y, float x );

/*********************************
** Dealing with junk
********************************/

// Order:
// Timestamp, accel[0:2], gyro[0:2], yaw, pitch, roll, aest_ave[0:1], vest_ave[0:1]
// Timestamp in micro seconds
typedef struct
{
    unsigned long timestamp;
    float accel[3];
    float gyro[3];
    bool flag;
} EMULATE_TYPE;

#endif // EMULATOR








