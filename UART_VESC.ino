//Library for VESC UART
#include <VescUartControl.h>

// global variables
#import "global_variables.h"

#define USE_PATCHED_VESC_FW_2_18

unsigned long count;

bool VESC_setup()
{
  Vesc_Right.begin(&Serial2);
  Vesc_Right.UartSetCurrent(0.0);
  Vesc_Right.UartSetCurrentBrake(0.0);

  Serial.println("VESC_setup_right_begin");
  delay(200);

#if defined (USE_PATCHED_VESC_FW_2_18)
  if (Vesc_Right.UartGetLimits(limitsVescRight))
  {
    Serial.print("currentMax_Vesc_Right: "); Serial.println(limitsVescRight.currentMax);
    Serial.print("currentMin_Vesc_Right: "); Serial.println(limitsVescRight.currentMin);
    Serial.print("inCurrentMax_Vesc_Right: "); Serial.println(limitsVescRight.inCurrentMax);
    Serial.print("inCurrentMin_Vesc_Right: "); Serial.println(limitsVescRight.inCurrentMin);
    Serial.print("absCurrentMax_Vesc_Right: "); Serial.println(limitsVescRight.absCurrentMax);
    Serial.print("minVin_Vesc_Right: "); Serial.println(limitsVescRight.minVin);
    Serial.print("maxVin_Vesc_Right: "); Serial.println(limitsVescRight.maxVin);
    Serial.print("batteryCutStart_Vesc_Right: "); Serial.println(limitsVescRight.batteryCutStart);
    Serial.print("batteryCutEnd_Vesc_Right: "); Serial.println(limitsVescRight.batteryCutEnd);
    Serial.print("tempFetStart_Vesc_Right: "); Serial.println(limitsVescRight.tempFetStart);
    Serial.print("tempFetEnd_Vesc_Right: "); Serial.println(limitsVescRight.tempFetEnd);
    Serial.print("tempMotStart_Vesc_Right: "); Serial.println(limitsVescRight.tempMotorStart);
    Serial.print("tempMotEnd_Vesc_Right: "); Serial.println(limitsVescRight.tempMotorEnd);
    Serial.print("max_Duty_Vesc_Right: "); Serial.println(limitsVescRight.max_Duty); 
    VESC_right_limits_ok = 1;
  }
  else
  {
    Serial.println("could not get limits from Vesc_Right");
    VESC_right_limits_ok = 0;
  }
#else
  limitsVescRight.currentMax=MAX_AMP_MOTOR;
  limitsVescRight.currentMin=-MAX_AMP_MOTOR;
#endif //#if defined (USE_PATCHED_VESC_FW_2_18)
  Serial.println("VESC_setup_right_end");
  delay(100);

  Vesc_Left.begin(&Serial3);
  Vesc_Left.UartSetCurrent(0.0);
  Vesc_Left.UartSetCurrentBrake(0.0);

  Serial.println("VESC_setup_left_begin");
  delay(200);

#if defined (USE_PATCHED_VESC_FW_2_18)
  if (Vesc_Left.UartGetLimits(limitsVescLeft))
  {
    Serial.print("currentMax_Vesc_Left: "); Serial.println(limitsVescLeft.currentMax);
    Serial.print("currentMin_Vesc_Left: "); Serial.println(limitsVescLeft.currentMin);
    Serial.print("inCurrentMax_Vesc_Left: "); Serial.println(limitsVescLeft.inCurrentMax);
    Serial.print("inCurrentMin_Vesc_Left: "); Serial.println(limitsVescLeft.inCurrentMin);
    Serial.print("absCurrentMax_Vesc_Left: "); Serial.println(limitsVescLeft.absCurrentMax);
    Serial.print("minVin_Vesc_Left: "); Serial.println(limitsVescLeft.minVin);
    Serial.print("maxVin_Vesc_Left: "); Serial.println(limitsVescLeft.maxVin);
    Serial.print("batteryCutStart_Vesc_Left: "); Serial.println(limitsVescLeft.batteryCutStart);
    Serial.print("batteryCutEnd_Vesc_Left: "); Serial.println(limitsVescLeft.batteryCutEnd);
    Serial.print("tempFetStart_Vesc_Left: "); Serial.println(limitsVescLeft.tempFetStart);
    Serial.print("tempFetEnd_Vesc_Left: "); Serial.println(limitsVescLeft.tempFetEnd);
    Serial.print("tempMotStart_Vesc_Left: "); Serial.println(limitsVescLeft.tempMotorStart);
    Serial.print("tempMotEnd_Vesc_Left: "); Serial.println(limitsVescLeft.tempMotorEnd);
    Serial.print("max_Duty_Vesc_Left: "); Serial.println(limitsVescLeft.max_Duty); 
    VESC_left_limits_ok = 1;
  }
  else
  {
    Serial.println("could not get limits from Vesc_Left");
    VESC_left_limits_ok = 0;
  }
#else
  limitsVescLeft.currentMax=MAX_AMP_MOTOR;
  limitsVescLeft.currentMin=-MAX_AMP_MOTOR;
#endif //#if defined (USE_PATCHED_VESC_FW_2_18)
  Serial.println("VESC_setup_left_end");
  delay(100);
  
  if (VESC_right_limits_ok==1 & VESC_left_limits_ok==1) {return true;}
  else {return false;}
  
}

void VESC_update()
{
  Vesc_Right.UartSetCurrent(mapfloat(power_right, -MAX_PID, MAX_PID, limitsVescRight.currentMin, limitsVescRight.currentMax));
  Vesc_Left.UartSetCurrent(mapfloat(power_left, -MAX_PID, MAX_PID, limitsVescLeft.currentMin, limitsVescLeft.currentMax));
}

bool VESC_read()
{
  
  if (Vesc_Right.UartGetValue(measuredVescRight))
  {
    VESC_right_measurements_ok=1;
  }
  else
  {
    VESC_right_measurements_ok=0;
  }


  if (Vesc_Left.UartGetValue(measuredVescLeft))
  {
    VESC_left_measurements_ok=1;
  }
  else
  {
    VESC_left_measurements_ok=0;
  }


  if(VESC_right_measurements_ok ==1 || VESC_left_measurements_ok == 1)
  {
    return true;
  }
  else
  {
    return false;
  }
}

