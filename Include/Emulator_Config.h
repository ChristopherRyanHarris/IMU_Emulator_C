
/*******************************************************************
** FILE: 
**   	Emulator.h
** DESCRIPTION: 
** 		This file contains all header information specific to the
** 		the IMU emulator. The IMU emulator uses previously collected
** 		data. This allows us to test the preformance of the algorithm
******************************************************************/

#ifndef EMULATOR_CONFIG_H
#define EMULATOR_CONFIG_H


/*******************************************************************
** Imcludes ********************************************************
********************************************************************/


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








