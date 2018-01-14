
/******************************************************************
** FILE: DCM_Config.h
** Header file for the Directional Cosine Matrix (DCM) "filter"
** and orientation algorithms.
** This file should contain only definitions specific to the
** DCM algorithms
******************************************************************/

#ifndef DCM_H
#define DCM_H


/*******************************************************************
** Tyedefs *********************************************************
********************************************************************/

/*
** TYPE: DCM_STATE_TYPE
** This type is used to hold the DCM
** specific arrays and variables */
typedef struct
{
  float Omega_P[3];
  float Omega_I[3];
  float DCM_Matrix[3][3];
  
  float gyro_ave[3];
  float gyro_var[3];
  float gyro_std[3];
  
  float std_time;
  
  long int SampleNumber;
} DCM_STATE_TYPE;



#endif