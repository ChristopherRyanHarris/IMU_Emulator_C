#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <inttypes.h>
#include <stdbool.h>

#include "Emulator.h"

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
DSP_COMMON_TYPE     g_dsp;

EMULATE_TYPE        g_emu_data;

// Order:
// Timestamp, accel[0:2], gyro[0:2], yaw, pitch, roll, aest_ave[0:1], vest_ave[0:1]
// Timestamp in micro seconds
const char* EmuFile="C:\\Users\\Christopher Harris\\Desktop\\Collect05032017\\Data\\Becca_1p8_0pct.bin";
FILE *g_fid;

int main()
{
  float count = 1.0;
  g_fid = fopen(EmuFile,"rb");
  g_emu_data.flag=1;

  Read_Sensors();
  Reset_Sensor_Fusion();
  Common_Init();
  Init_Emulator();
  WISE_Init();
  //DSP_Filter_Init();

  while( g_emu_data.flag==1 )
  {
    Read_Sensors();
    //FIR_Filter();
    //IIR_Filter();
    //DSP_Shift();
    if (g_emu_data.flag==0)
      continue;

    Update_Time();
    DCM_Filter();
    WISE_Update();

		//*2.23694
    fprintf(stdout,"Count: %f\n",count);
    fprintf(stdout,"Time:%ld\t",g_control_state.timestamp);
    fprintf(stdout,"Dt:%f\n",g_control_state.G_Dt);
    fprintf(stdout,"yaw:%f\t pitch:%f\t roll:%f\n",TO_DEG(g_sensor_state.yaw),TO_DEG(g_sensor_state.pitch),TO_DEG(g_sensor_state.roll));
    fprintf(stdout,"accel[0]:%f accel[1]:%f accel[2]:%f\n",g_sensor_state.accel[0],g_sensor_state.accel[1],g_sensor_state.accel[2]);
    //fprintf(stdout,"accel[0]:%f accel[1]:%f accel[2]:%f\n",(GTOMPS2*g_sensor_state.accel[0])/GRAVITY,(GTOMPS2*g_sensor_state.accel[1])/GRAVITY,(GTOMPS2*g_sensor_state.accel[2])/GRAVITY);
    fprintf(stdout,"gyro[0]:%f gyro[1]:%f gyro[2]:%f\n",g_sensor_state.gyro[0],g_sensor_state.gyro[1],g_sensor_state.gyro[2]);
    fprintf(stdout,"aveAcc[0]:%f aveAcc[1]:%f\n",g_wise_state.accel_ave[0],g_wise_state.accel_ave[1]);
    fprintf(stdout,"aveVel[0]:%f aveVel[1]:%f\n",g_wise_state.vel_ave[0],g_wise_state.vel_ave[1]);
    fprintf(stdout,"vel[0]:%f vel[1]:%f\n",g_wise_state.vel[0],g_wise_state.vel[1]);
    fprintf(stdout,"\n");
    getchar();
    count++;
  }
  fclose(g_fid);


  return 0;
}


/*************************************************
** Init_Emulator
*/
void Init_Emulator( void )
{
  LOG_PRINT("> Initializing\n");

  g_control_state.timestamp      = 0;
  g_control_state.timestamp_old  = 0;
  g_control_state.G_Dt           = 0.0;

  g_control_state.g_BaudLock       = true;
  g_control_state.g_LedState       = false;
  g_control_state.g_LastBlinkTime  = 0;
}

/*************************************************
** Read_Sensors
*/
void Read_Sensors( void )
{
  float temp;
  int count;

  count = fread(&temp,sizeof(float),1,g_fid);
  if ( count != 1 )
  {
    fprintf(stdout,"Emulator Exit\n");
    g_emu_data.flag = 0;
    return;
  }

  g_emu_data.timestamp = (unsigned long) temp;
  fread(&g_sensor_state.accel[0],sizeof(float),3,g_fid);
  fread(&g_sensor_state.gyro[0],sizeof(float),3,g_fid);
  fseek(g_fid,sizeof(float)*7,SEEK_CUR);
}
