
/*************************************************
** FILE: main
** This is the calling executable for the emulation
** code. This can only be used offline, and with the
** proper emulation-related flags set appropriatly.
*/


/*******************************************************************
** Includes ********************************************************
********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <inttypes.h>
#include <stdbool.h>

#include "../Include/Emulator_Config.h"

void Init_Emulator( void );
void Read_Sensors( void );

/*******************************************************************
** Globals *********************************************************
********************************************************************/

/* DCM variables */
CAL_STATE_TYPE      g_calibration;
DCM_STATE_TYPE      g_dcm_state;
SENSOR_STATE_TYPE   g_sensor_state;
CONTROL_STATE_TYPE  g_control_state;
WISE_STATE_TYPE     g_wise_state;
GAPA_STATE_TYPE     g_gapa_state;
DSP_COMMON_TYPE     g_dsp;

EMULATE_TYPE        g_emu_data;

// Order:
// Timestamp, accel[0:2], gyro[0:2], yaw, pitch, roll, aest_ave[0:1], vest_ave[0:1]
// Timestamp in micro seconds
//const char* g_EmuFile = ".\\Data\\BinaryData\\Subject1\\Subject1_2p7_0pct.bin";
const char* g_EmuFile = ".\\Data\\BinaryData\\Subject3_2\\F4_2.bin";
FILE *      g_fid;


/*******************************************************************
** START ***********************************************************
********************************************************************/

int main()
{
  float count = 1.0;
  int i,j;

  g_fid = fopen(g_EmuFile,"rb");
  g_emu_data.flag=1;

  Read_Sensors();
  Reset_Sensor_Fusion();
  Common_Init();
  Init_Emulator();
  WISE_Init();
  GaPA_Init();
  DSP_Filter_Init();

  fprintf(stdout, " accel:\t");
  for(i=0;i<3;i++)
  {
  	fprintf(stdout," %f ",g_sensor_state.accel[i]);
  }
  fprintf(stdout,"\n");
  fprintf(stdout, " gyro:\t");
  for(i=0;i<3;i++)
  {
  	fprintf(stdout," %f ",g_sensor_state.gyro[i]);
  }
  fprintf(stdout,"\n");

  for(i=0;i<3;i++)
  {
  	for(j=0;j<3;j++)
  	{
  		fprintf(stdout," %f ",g_dcm_state.DCM_Matrix[i][j]);
  	}
  	fprintf(stdout,"\n");
  }
  fprintf(stdout,"pitch:%f\n",g_sensor_state.pitch);

  while( g_emu_data.flag==1 )
  {
    Read_Sensors();
    if (g_emu_data.flag==0)
      continue;

    //FIR_Filter();
    //IIR_Filter();
    //DSP_Shift();

    Update_Time();
    DCM_Filter();
    GaPA_Update();
    //WISE_Update();

    fprintf(stdout,"\n");
    fprintf(stdout,"Count: %f\n",count);
    fprintf(stdout,"Time:%ld\t",g_control_state.timestamp);
    fprintf(stdout,"dt:%f\n",g_control_state.G_Dt);

    fprintf(stdout, " accel:\t");
    for(i=0;i<3;i++)
    {
    	fprintf(stdout," %f ",g_sensor_state.accel[i]);
    }
    fprintf(stdout,"\n");
    fprintf(stdout, " gyro:\t");
    for(i=0;i<3;i++)
    {
    	fprintf(stdout," %f ",g_sensor_state.gyro[i]);
    }
    fprintf(stdout,"\n");

    for(i=0;i<3;i++)
    {
    	for(j=0;j<3;j++)
    	{
    		fprintf(stdout," %f ",g_dcm_state.DCM_Matrix[i][j]);
    	}
    	fprintf(stdout,"\n");
    }
    fprintf(stdout,"pitch:%f\n",g_sensor_state.pitch);

    fprintf(stdout,"nu:%f\n",g_gapa_state.nu);
    fprintf(stdout,"phi:%f\t PHI:%f\n",g_gapa_state.phi, g_gapa_state.PHI);
    fprintf(stdout,"phi_mw:%f\t PHI_mw:%f\n",g_gapa_state.phi, g_gapa_state.PHI);
    fprintf(stdout,"phi_max:%f\t PHI_max:%f\n",g_gapa_state.phi_max, g_gapa_state.PHI_max);
    fprintf(stdout,"phin:%f\t PHIn:%f\n",g_gapa_state.phin, g_gapa_state.PHIn);
    fprintf(stdout,"z_phi:%f\t z_PHI:%f\t",g_gapa_state.z_phi, g_gapa_state.z_PHI);
    //fprintf(stdout,"Dt:%f\n",g_control_state.G_Dt);
    //fprintf(stdout,"yaw:%f\t pitch:%f\t roll:%f\n",TO_DEG(g_sensor_state.yaw),TO_DEG(g_sensor_state.pitch),TO_DEG(g_sensor_state.roll));
    //fprintf(stdout,"accel[0]:%f accel[1]:%f accel[2]:%f\n",g_sensor_state.accel[0],g_sensor_state.accel[1],g_sensor_state.accel[2]);
    //fprintf(stdout,"gyro[0]:%f gyro[1]:%f gyro[2]:%f\n",g_sensor_state.gyro[0],g_sensor_state.gyro[1],g_sensor_state.gyro[2]);
    //fprintf(stdout,"aveAcc[0]:%f aveAcc[1]:%f\n",g_wise_state.accel_ave[0],g_wise_state.accel_ave[1]);
    //fprintf(stdout,"Acc[0]:%f Acc[1]:%f\n",g_wise_state.accel[0],g_wise_state.accel[1]);
    //fprintf(stdout,"aveVel[0]:%f aveVel[1]:%f\n",g_wise_state.vel_ave[0],g_wise_state.vel_ave[1]);
    //fprintf(stdout,"vel[0]:%f vel[1]:%f\n",g_wise_state.vel[0],g_wise_state.vel[1]);
    fprintf(stdout,"\n");
    //getchar();
    count++;
  }
  fclose(g_fid);


  return 0;
} /* End Main */


