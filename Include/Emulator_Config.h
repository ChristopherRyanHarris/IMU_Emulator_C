
/*******************************************************************
** FILE: 
**   	Emulator_Config.h
** DESCRIPTION: 
** 		This file contains all header information specific to the
** 		the IMU emulator. The IMU emulator uses previously collected
** 		data. This allows us to test the preformance of the algorithm
********************************************************************/
#ifndef EMULATOR_CONFIG_H
#define EMULATOR_CONFIG_H


/*******************************************************************
** Imcludes ********************************************************
********************************************************************/


/*******************************************************************
** Defines *********************************************************
********************************************************************/

#define LOG_HEADER fprintf(stdout,"[%s:%d] > ",__func__,__LINE__);
#define LOG_PRINTLN(x) LOG_HEADER;fprintf(stdout,x);fprintf(stdout,"\n");
#define LOG_PRINT(x) LOG_HEADER;fprintf(stdout,x);

#define COMM_PRINT(x)  LOG_HEADER;fprintf(stdout,"[COMM PRINT]: %c\n",x);
#define COMM_WRITE(x,y)  LOG_HEADER;fprintf(stdout,"[COMM WRITE]: %d (len:%d)",*x,y);
#define COMM_AVAILABLE (0)
#define COMM_READ (-1) /*Serail.read() returns "-1" if no data */

/*******************************************************************
** Tyedefs *********************************************************
********************************************************************/

typedef struct
{
    unsigned long timestamp;
    float accel[3];
    float gyro[3];

    bool flag;

    FILE* InputFID;
    char*	InputFile;
    FILE* OutputFID;
    char* OutputFile;
    
} EMULATION_TYPE;

#endif /* End EMULATOR_CONFIG_H */








