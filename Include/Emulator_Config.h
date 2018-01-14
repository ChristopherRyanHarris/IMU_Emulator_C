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

//#include "Common_Config.h"

//#ifdef _IMU10736_
//#include "../Include/IMU10736_Config.h"
//#endif
//#ifdef _IMU9250_
//#include "../Include/IMU9250_Config.h"
//#endif

//#include "DSP_Config.h"
//#include "WISE_Config.h"
//#include "GaPA_Config.h"

/*******************************************************************
** Tyedefs *********************************************************
********************************************************************/

typedef struct
{
    unsigned long timestamp;
    float accel[3];
    float gyro[3];
    
    bool flag;
    
    FILE*       InputFID;
    FILE*       OutputFID;
} EMULATION_TYPE;

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

/* GaPA_Functions */
void GaPA_Init( void );
void GaPA_Reset( void );
void GaPA_Update( void );
void TrackPhiVariables( GAPA_STATE_TYPE* l_gapa_state );
void calc_SftPrmLeft( float* GAMMA, float PHI_max, float PHI_min );
void calc_SftPrmRight( float* gamma, float phi_max, float phi_min );
void calc_ScaleFactor( float *z, float phi_max, float phi_min, float PHI_max, float PHI_min );
void calc_PhaseAngle( float* nu, float z, float PHI, float GAMMA, float phi, float gamma );

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
float Windowed_Mean( float m, float x, int n, float a );
float f_asin( float x );
float f_atan2( float y, float x );
void calc_circle_center( float p1[2], float p2[2], float p3[2], float xcyc[2] );


#endif /* EMULATOR */








