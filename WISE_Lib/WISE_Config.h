
/******************************************************************
** FILE: WISE_Common.h
** Header for the Walking Speed and Incline Estimator (WISE)
** This file should contain only definitions specific to the
** WISE algorithms
******************************************************************/

#ifndef WISE_COMMON_H
#define WISE_COMMON_H

//#define WISE_GAIN_AD 0.025f
#define WISE_GAIN_AD 0.05f
//#define WISE_GAIN_AP 0.00005f
#define WISE_GAIN_AP 0.0f

//#define WISE_GAIN_VD 0.25f
#define WISE_GAIN_VD 0.3f
#define WISE_GAIN_VP 0.03f
//#define WISE_GAIN_VP 0.0f

#define WISE_CORRECTION 3.720962

#define WISE_MINCOUNT 20

/*******************************************************************
** Tyedefs
********************************************************************/

/*
** TYPE: WISE_STATE_TYPE
** This holds all the state variables
** for the walking speed and estimation code
*/
typedef struct
{
  bool swing_state; // 0:down 1:up
  bool toe_off;
  int minCount;

  float GaitStart[5]; /* [Vel,SampleNo] */
  float GaitEnd[5]; /* [Vel,SampleNo] */

  float pitch_mem;
  float pitch_delta;
  float pitch_delta_total;

  /* [a_x, a_y, a_z]
  ** Leg acceleration vector
  ** wrt leg coordinate reference frame */
  float accel[3];
  float accel_total[3];
  float accel_ave[3];
  float accel_min[3];
  float accel_max[3];

  float accel_delta[3];
  float omega_ad[3];
  float omega_ap[3];

  /* [v_x, v_y, v_Z]
  ** Leg velocity vector
  ** wrt leg coordinate reference frame */
  float vel[3];
  float vel_total[3];
  float vel_ave[3];
  float vel_min[3];
  float vel_max[3];

  float vel_delta[3];
  float omega_vd[3];
  float omega_vp[3];

  float pe[3];
  float pave;

  float Nsample;
  float Ncycles;
} WISE_STATE_TYPE;


#endif // WISE_COMMON_H


