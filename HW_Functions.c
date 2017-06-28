/*************************************************
** FILE : HW_Functions
** This file contains functions which are used
** to asses the internal state of the board.
** Ideally, we would have functions which can run
** diagnostics.
** All functions should be platform independent
**************************************************/


#include "Common_Config.h"
#if EXE_MODE /* Emulator mode */
#include "DSP_Lib/DSP_Config.h"
#include "WISE_Lib/WISE_Config.h"
#include "IMU10736_Lib/IMU10736_Config.h"

#include "Emulator_Config.h"
extern CAL_STATE_TYPE      g_calibration;
extern DCM_STATE_TYPE      g_dcm_state;
extern DSP_COMMON_TYPE     g_dsp;
extern SENSOR_STATE_TYPE   g_sensor_state;
extern CONTROL_STATE_TYPE  g_control_state;
extern WISE_STATE_TYPE     g_wise_state;
#endif

/*************************************************
** Init_Hardware
** This function sets the LED GPIO Pin and
** the com port baud rates.
*/
void Init_Hardware( void )
{
  /* Some Log Output (usb) */
  LOG_PRINTLN("> Initializing Hardware");

  #if EXE_MODE==0 /* IMU Mode */
  /* Initiate the LOG_PORT */
  LOG_PORT.begin(LOG_PORT_BAUD);

  /* Set up LED pin (active-high, default to off) */
  pinMode(HW_LED_PIN, OUTPUT);
  digitalWrite(HW_LED_PIN, LOW);
  #endif

  g_control_state.timestamp      = 0;
  g_control_state.timestamp_old  = 0;
  g_control_state.G_Dt           = 0.0;

  g_control_state.g_BaudLock       = true;
  g_control_state.g_LedState       = false;
  g_control_state.g_LastBlinkTime  = 0;
}

/*************************************************
** BLinkLED
** This function is used to communicate to the user
** that the board is indeed doing things
** TO DO: I plan to implement a blink code for debugging
*/
void Blink_LED( void )
{
  #if EXE_MODE==0 /* IMU Mode */
  /* We blink every UART_BLINK_RATE millisecods */
  if ( millis() > (g_control_state.g_LastBlinkTime + UART_BLINK_RATE) )
  {
    /* Log the current states to the debug port */
    Debug_LogOut();

    /* Display number of bytes available on comm port
    ** Com port is used for real-time communication with
    ** connected processor */
    LOG_PORT.println("> # Available on COMM_PORT: " + String(COMM_PORT.available()) );

    /* Toggle LED */
    LOG_PORT.println("> Blink ...");
    digitalWrite(HW_LED_PIN, g_control_state.g_LedState);
    g_control_state.g_LedState = !g_control_state.g_LedState;
    g_control_state.g_LastBlinkTime = millis();
  }
  #endif /* EXE_MODE */
} /* End f_BLinkLED */




