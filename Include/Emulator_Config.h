/******************************************************************
** FILE: Emulator.h
** This file contains all header information specific to the
** the IMU emulator. The IMU emulator uses previously collected
** data. This allows us to test the preformance of the algorithm
******************************************************************/

#ifndef EMULATOR_CONFIG_H
#define EMULATOR_CONFIG_H


/*******************************************************************
** Imcludes ********************************************************
********************************************************************/

#include "Common_Config.h"

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

#endif /* EMULATOR_CONFIG_H */








