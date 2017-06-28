/*************************************************
** FILE: WISE_Functions
** This file contains all functions which are
** specific to the speed and walking incline
** estimation capabilities
**************************************************/

#include "../Common_Config.h"

#if EXE_MODE /* Emulator mode */
#include <math.h>
#include "../Math.h"
#include "../DSP_Lib/DSP_Config.h"
#include "WISE_Config.h"
#include "../IMU10736_Lib/IMU10736_Config.h"

#include "../Emulator_Config.h"
extern CAL_STATE_TYPE      g_calibration;
extern DCM_STATE_TYPE      g_dcm_state;
extern DSP_COMMON_TYPE     g_dsp;
extern SENSOR_STATE_TYPE   g_sensor_state;
extern CONTROL_STATE_TYPE  g_control_state;
extern WISE_STATE_TYPE     g_wise_state;
#endif

/*
** Function: WISE_Init
** This function initializes the WISE state
** variables.
*/
void WISE_Init ( void )
{
  int i;

  g_wise_state.swing_state = FALSE; /* Bool */
  g_wise_state.toe_off     = FALSE; /* Bool */
  g_wise_state.minCount    = WISE_MINCOUNT;

  g_wise_state.Nsample = 1.0f;
  g_wise_state.Ncycles = 0.0f;

  g_wise_state.pitch_mem         = g_sensor_state.pitch;
  g_wise_state.pitch_delta       = 0.0f;
  g_wise_state.pitch_delta_total = 0.0f;

  g_wise_state.GaitStart[0] = 999;
  g_wise_state.GaitStart[1] = 999;
  g_wise_state.GaitStart[2] = 999;
  g_wise_state.GaitStart[3] = 999;
  g_wise_state.GaitStart[4] = 999;
  
  g_wise_state.GaitEnd[0]   = 999;
  g_wise_state.GaitEnd[1]   = 999;
  g_wise_state.GaitEnd[2]   = 999;
  g_wise_state.GaitEnd[3]   = 999;
  g_wise_state.GaitEnd[4]   = 999;

  for( i=0;i<3;i++ )
  {
    /* Initialize WISE Acceleration state vector */
    g_wise_state.accel_ave[i]   =  1.0f;
    g_wise_state.accel[i]       =  0.0f;
    g_wise_state.accel_total[i] =  0.0f;
    g_wise_state.accel_min[i]   =  999.0f;
    g_wise_state.accel_max[i]   = -999.0f;

    /* Initialize WISE Velocity state vector */
    g_wise_state.vel_ave[i] = 1.0f;
    g_wise_state.vel[i]     = 0.0f;
    //g_wise_state.vel_total[i] = 0.0f;

    g_wise_state.vel_delta[i] = 0.0f;
    g_wise_state.omega_vd[i]  = 0.0f;
    g_wise_state.omega_vp[i]  = 0.0f;
  }
  g_wise_state.Nsample++;
}

/*
** Function: WISE_Update
** This code executes the speed and
** walking incline estimation state update
*/
void WISE_Update ( void )
{
  g_wise_state.Nsample++;
  g_wise_state.pitch_delta = g_sensor_state.pitch - g_wise_state.pitch_mem;
  g_wise_state.pitch_delta_total += g_wise_state.pitch_delta;

  g_wise_state.swing_state = 1;

  /* Map acceleration to normal/tangent */
  Map_Accel_2D();
  /* Integrate accel to get vel */
  Integrate_Accel_2D();
  /* Velocity Adjustment */
  Adjust_Velocity();

  g_wise_state.pitch_mem = g_sensor_state.pitch;
}

/*
** Function: WISE_Reset
** This function resets the WISE
** state variables. In particular,
** the integrated variables.
*/
void WISE_Reset ( void )
{
  int i;

  g_wise_state.toe_off = FALSE;
  g_wise_state.Nsample = 1.0f;

  for( i=0; i<=2; i++)
  {
    /* Initialize WISE Acceleration state vector */
    g_wise_state.accel_ave[i]   =  1.0f;
    g_wise_state.accel[i]       =  0.0f;
    g_wise_state.accel_total[i] =  0.0f;
    g_wise_state.accel_min[i]   =  999.0f;
    g_wise_state.accel_max[i]   = -999.0f;

    /* Initialize WISE Velocity state vector */
    g_wise_state.vel_total[i] = 0.0f;
    g_wise_state.vel_delta[i] = 0.0f;
    g_wise_state.omega_vd[i]  = 0.0f;
    g_wise_state.omega_vp[i]  = 0.0f;
  }
} /* End WISE_Reset */


/*
** Function: Map_Accel_2D
** This function maps a_t and a_n to a_x and a_y
** using the filtered pitch assuming 2D motion.
** This Does not account for roll.
** NOTE: There may be a better way of extracting
**       this from the mid-filter DCM
*/
void Map_Accel_2D ( void )
{
  /*
  ** Notes on orientation for the 10736 IMU
  **   Terms:
  **     Fore:       (Front) Edge oposite of the power port
  **     Aft:        (Rear) Edge of the power port
  **     Starboard:  (Right) Edge with reset switch
  **     Port:       (Left) Edge oposite of reset switch
  **     Zenith:     (Up) Clean face of board
  **     Nadir:      (Down) Populated face of board
  **   Contrary to the silk, the axis are positioned as follows:
  **     +x is Fore,       -x is Aft
  **     +y is Starboard,  -y is Port
  **     +z is Zenith,     -z is Nadir
  */

  /* Accel x:Fore y:Port z:Zenith
  ** Note: IMU coordinate ref. frame definced in IMU#_Config.h
  **       Rotation will need to be accounted for */
  float Ax, Az;
  Ax = g_sensor_state.accel[0];
  Az = g_sensor_state.accel[2];

  g_wise_state.accel_delta[0] = Ax;
  g_wise_state.accel_delta[1] = Az;

  /**********************************
  ** Tangent Part *******************
  **********************************/

  /* Calc wrt world coordinate system */
  g_wise_state.accel[0]   = (Ax*cos(g_sensor_state.pitch) - Az*sin(g_sensor_state.pitch)) * GTOMPS2/GRAVITY * MPSTOMPH * WISE_CORRECTION;

  /* Feedback */
  g_wise_state.accel_delta[0] = g_wise_state.accel[0] - g_wise_state.accel_delta[0];
  g_wise_state.omega_ad[0]    = g_wise_state.accel_delta[0]*WISE_GAIN_AD;
  g_wise_state.omega_ap[0]    = g_wise_state.accel[0]*WISE_GAIN_AP;

  /* Get average */
  g_wise_state.accel_total[0] += g_wise_state.accel[0];
  g_wise_state.accel_ave[0]  = g_wise_state.accel_total[0]/g_wise_state.Nsample;

  /* log min & max */
  if( g_wise_state.accel_total[0]<g_wise_state.accel_min[0] )
  {
    g_wise_state.accel_min[0] = g_wise_state.accel_ave[0];
    g_wise_state.accel_max[0] = g_wise_state.accel_ave[0];
  }
  if( g_wise_state.accel_total[0]>g_wise_state.accel_max[0] ) { g_wise_state.accel_max[0] = g_wise_state.accel_ave[0]; }

  /*********************************
  ** Normal Part *******************
  **********************************/

  /* Calc Ay wrt world coordinate system */
  g_wise_state.accel[1]  = -(Ax*sin(g_sensor_state.pitch) - Az*cos(g_sensor_state.pitch) + 1) * GTOMPS2/GRAVITY * MPSTOMPH;

  /* Feedback */
  g_wise_state.accel_delta[1] = g_wise_state.accel[1] - g_wise_state.accel_delta[1];
  g_wise_state.omega_ad[1]    = g_wise_state.accel_delta[1]*WISE_GAIN_AD;
  g_wise_state.omega_ap[1]    = g_wise_state.accel[1]*WISE_GAIN_AP;

  /* Get average */
  g_wise_state.accel_total[1] += g_wise_state.accel[1];
  g_wise_state.accel_ave[1]  = g_wise_state.accel_total[1]/g_wise_state.Nsample;

  /* log min & max */
  if( g_wise_state.accel_total[1]<g_wise_state.accel_min[1] )
  {
    g_wise_state.accel_min[1] = g_wise_state.accel_ave[1];
    g_wise_state.accel_max[1] = g_wise_state.accel_ave[1];
  }
  if( g_wise_state.accel_total[1]>g_wise_state.accel_max[1] ) { g_wise_state.accel_max[1] = g_wise_state.accel_ave[1]; }
} /* End Map_Accel_2D */



/*
** Function: Integrate_Accel_2D
** Integrate acceleration (wrt leg ref coordinates)
** to get velocity (wrt leg ref coordinates)
** Assumes 2D motion
*/
void Integrate_Accel_2D ( void )
{
  int i;
  for( i=0; i<=2; i++)
  {
    g_wise_state.vel_delta[i] = g_wise_state.vel[i];

    //g_wise_state.vel[i]  = g_wise_state.vel[i] + g_control_state.G_Dt*(g_wise_state.accel_max[i]-g_wise_state.accel_min[i]);
    //g_wise_state.vel[i]  = g_wise_state.vel[i] + g_wise_state.accel_ave[i]*g_control_state.G_Dt;
    g_wise_state.vel[i]  = g_wise_state.vel[i] + g_wise_state.accel[i]*g_control_state.G_Dt;

    //g_wise_state.vel[i] -= (g_wise_state.omega_vd[i] + g_wise_state.omega_vp[i]);
    g_wise_state.vel_delta[i] = g_wise_state.vel[i] - g_wise_state.vel_delta[i];
    g_wise_state.omega_vd[i]  = g_wise_state.vel_delta[i]*WISE_GAIN_VD;
    g_wise_state.omega_vp[i]  = g_wise_state.vel[i]*WISE_GAIN_VP;

    g_wise_state.vel_total[i] += g_wise_state.vel[i];
    //g_wise_state.vel_ave[i]  = g_wise_state.vel_total[i]/g_wise_state.Nsample;
  }
} /* End Integrate_Accel_2D */


/*
** Function: Adjust_Velocity
** This function adjusts for velocity drift.
** We detect toe-off events, from there we
** can determine an estimated drift over the previous
** gait cycle. Then we can adjust the average velocity.
*/
void Adjust_Velocity( void )
{
  float slope = 1.0f;
  float NGaitSamples = 0.0f;
  
  /* Part 0: Set gait start for first cycle */
  if( (g_wise_state.Ncycles==0) & (g_wise_state.Nsample==1) )
  {
    g_wise_state.GaitStart[0] = g_wise_state.vel[0];
    g_wise_state.GaitStart[1] = g_wise_state.vel[1];
    g_wise_state.GaitStart[2] = g_wise_state.vel_total[0];
    g_wise_state.GaitStart[3] = g_wise_state.vel_total[1];
    g_wise_state.GaitStart[4] = g_wise_state.Nsample;
  }

  /* Part I : At toe-off, we must correct velocity measured
  ** We must wait a few full gait cycles to get
  ** a good slope estimate.
  ** We must be within, at a minimum, the second full
  ** gait cycle.
  ** GaitStart[0] is initialized to 999
  ** GaitStart[0] is reset within the first full gait cycle */
  if ( (g_wise_state.Nsample-g_wise_state.GaitEnd[4])>(g_wise_state.minCount) )
  {
  	fprintf(stdout,"DEBUG - Toe off! S:%f vel[0]:%f vel[1]:%f\n",g_wise_state.GaitEnd[4],g_wise_state.GaitEnd[0],g_wise_state.GaitEnd[1]);
  	
    /* We must be within, at a minimum, the second
    ** gait cycle. This garuntees we are calculating the
    ** slope from toe-off to toe-off
    ** GaitStart[1] is initialized to -1
    ** GaitStart[1] is reset within the first full gait cycle */
    if( g_wise_state.Ncycles>=1 )
    {
    	NGaitSamples = g_wise_state.GaitEnd[4] - g_wise_state.GaitStart[4];
    	
      /* We correct for the drift by approximating
      ** the drift over the gait cycle.
      ** We can then subtract the accumulated drift
      ** over the cycle and the dc bias.
      ** GaitStart = {vel0_i, vel1_i}
      ** This assumes we reset N at each toeoff */
      slope = ( g_wise_state.GaitEnd[0]-g_wise_state.GaitStart[0] )/NGaitSamples;
      g_wise_state.vel_ave[0]   = ( (g_wise_state.GaitStart[2]) - (slope*0.5*NGaitSamples*NGaitSamples) - (g_wise_state.GaitStart[0]*NGaitSamples) )/NGaitSamples;

      slope = ( g_wise_state.GaitEnd[1]-g_wise_state.GaitStart[1] )/NGaitSamples;
      g_wise_state.vel_ave[1]   = ( (g_wise_state.GaitStart[3]) - (slope*0.5*NGaitSamples*NGaitSamples) - (g_wise_state.GaitStart[1]*NGaitSamples) )/NGaitSamples;
      
      fprintf(stdout,"DEBUG - Vel Estimate V[0]:%f V[1]:%f\n",g_wise_state.vel_ave[0],g_wise_state.vel_ave[1]);
    }
    
    /* Reset saved minima and increment cycle counter */
    g_wise_state.Ncycles++;
    g_wise_state.GaitStart[0] = g_wise_state.GaitEnd[0];
    g_wise_state.GaitStart[1] = g_wise_state.GaitEnd[1];
    g_wise_state.GaitStart[2] = g_wise_state.GaitEnd[2];
    g_wise_state.GaitStart[3] = g_wise_state.GaitEnd[3];
    g_wise_state.GaitStart[4] = 1.0f;
    
    g_wise_state.GaitEnd[0]   = (999);
    g_wise_state.GaitEnd[1]   = (999);
    g_wise_state.GaitEnd[2]   = (999);
    g_wise_state.GaitEnd[3]   = (999);
    g_wise_state.GaitEnd[4]   = (999);

    /* Reset gait parameters
    ** for next cycle */
    WISE_Reset();
  }

  /* Part II : Record minima
  ** Here we are attempting to locate the local minima
  ** If this velocit is lower than the previous minima ... */
  if ( g_wise_state.vel[0] < g_wise_state.GaitEnd[0] )
  {
    /* If this sample is more than the minimal required */
    if ( (g_wise_state.Nsample-g_wise_state.GaitEnd[4]) > (g_wise_state.minCount) )
    { } /* This shouldn't happen */
    else
    {
    	fprintf(stdout,"DEBUG - Desending S:%f\n",g_wise_state.Nsample);
    	
      /* GaitStart[0] = { vel tangent at minima }
      ** GaitStart[1] = { vel normal at minima  } */
      g_wise_state.GaitEnd[0] = g_wise_state.vel[0];
      g_wise_state.GaitEnd[1] = g_wise_state.vel[1];
      g_wise_state.GaitEnd[2] = g_wise_state.vel_total[0];
      g_wise_state.GaitEnd[3] = g_wise_state.vel_total[1];
      g_wise_state.GaitEnd[4] = g_wise_state.Nsample;
    }
  }

  /* Part III : Update the min count threshold
  ** Once we have warmed up, we can try to
  ** estimate how far apart each gait is */
  if ( (g_wise_state.Ncycles>3) & (g_wise_state.Nsample==1) )
  {
    g_wise_state.minCount = ceil( ((NGaitSamples*0.4)+g_wise_state.minCount)*0.5 );
		fprintf(stdout,"DEBUG - Updating minCount:%d\n",g_wise_state.minCount);
  }
}




/*
** Function: Estimate_Error
** This function is inteded to estimate the
** error in the intitial velocity estimates.
** There are several ways we can estimate the
** error in the velocity:
**  1. We assume there is a relationship between the
**     velocity error and the pitch delta. I.e. at
**     small theta_dot, there is a high probability
**     of error in the estimate
**  2. We can take the value of the velocity feedback
**     intergation term as an approximation of the error.
**  3. We can take the difference between the final velocity and
**     the average velocity. This assumes that the
**     velocity should be a constant. Therefore, a large
**     difference may indicate that the velocity has error. */
void Estimate_Error ( void )
{
//  int i;
//  float pave,pe1,pe2,pe3;
//  float m1_pho, m1_ave_pitch_delta;
//  float m2_ave_feedback;
//  float m3_ave_vel, m3_delta;
//
//  /* Method 1
//  ** Estimate error from relationship to pitch delta
//  ** m1_pho: Strength of relationship */
//  m1_pho = 0.01f;
//  //m1_ave_pitch_delta = abs(g_wise_state.pitch_delta_total/g_wise_state.Nsample);
//  m1_ave_pitch_delta = fabs(g_wise_state.pitch_delta);
//  //g_wise_state.pe[0] = m1_ave_pitch_delta;
//  g_wise_state.pe[0] = exp(-m1_ave_pitch_delta/m1_pho);
//
//  /* Method 2:
//  ** Estimate error based on velocity feedback values */
//  m2_ave_feedback = 0.0f;
//  for( i=0;i<1;i++ ) { m2_ave_feedback += (g_wise_state.omega_vd[i]/g_wise_state.vel[i]); }
//  //for( i=0;i<2;i++ ) { m2_ave_feedback += abs(g_wise_state.omega_vd[i]/g_wise_state.vel[i]); }
//  g_wise_state.pe[1] = m2_ave_feedback;
//  //g_wise_state.pe[1] = min( abs(m2_ave_feedback/2), 1);
//
//  /* Method 3:
//  ** Estimate error based on the difference between
//  ** the average velocity and the final velocity */
//  m3_delta = 0.0f;
//  for( i=0;i<2;i++ )
//  {
//    m3_ave_vel = (g_wise_state.vel_total[i]/g_wise_state.Nsample);
//    m3_delta += fabs((m3_ave_vel - g_wise_state.vel[i])/m3_ave_vel);
//  }
//  //g_wise_state.pe[2] = m3_delta;
//  g_wise_state.pe[2] = fmin(m3_delta/2,1);
//
//  /* Get final error estimate
//  ** Final estimate can be the avereage p */
//  g_wise_state.pave = (g_wise_state.pe[0] + g_wise_state.pe[1] + g_wise_state.pe[2])/3;
//
//  /*
//  if ( millis() > (g_control_state.g_LastBlinkTime + UART_BLINK_RATE) )
//  {
//    imuLog = "\t\t err est (p1/p2/p3/pave): ";
//    imuLog += String( pe1,7 ) + "/" + String( pe2,7 ) + "/" + String( pe3,7 ) + "/" +String( pave,7 );
//    imuLog += "\r\n\n";
//    LOG_PRINT( imuLog );
//  } */
}

