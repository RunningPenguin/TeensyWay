#include <ADC.h>

// hardware settings file
#include "settings.h"

// IMU sensor libraries
#include <MPU6050_6Axis_MotionApps20.h>
#include <i2c_t3.h>
#include <MPU6050.h>
#include <EEPROM.h>
#include "DebugUtils.h"
#include "FreeIMU.h"
#include <Servo.h>

#include <PID.h>

// global variables
#include "global_variables.h"

// Indication of start o.k.
#include <Adafruit_NeoPixel.h>

//Library for VESC UART
#include <VescUartControl.h>

// Create three IntervalTimer objects
IntervalTimer task_high;
IntervalTimer task_mid;
IntervalTimer task_low;
IntervalTimer task_very_low;

ADC *adc = new ADC(); // adc object

FreeIMU fIMU = FreeIMU();

VescController Vesc_Right;
VescController Vesc_Left;

unsigned long countRight;
unsigned long countLeft;

void setup()
{
  noInterrupts();
  
  Wire.begin(I2C_MASTER, 0, I2C_PINS_18_19, I2C_PULLUP_EXT, I2C_RATE_800);
  delay(5);
  imu = MPU6050();
  imu.reset();
  delay(5);
  if (imu.init() == false)
  {
    SOS(0, "\tIMU not detected");
  }

  // initialize serial debug console
  debug_setup();

  if (RunMode == DevMode)
  {
    if (setupDMP() == false)
    {
      Serial.println("setupDMP error!");
      delay(100);
    }
  }
  else
  {
    //initialize Digital Motion Processor in the MPU6050
    setupDMP();
  }

  if (RunMode == DevMode)
  {
    if (PID_setup() == false)
    {
      Serial.println("PID_setup error!");
      delay(100);
    }
  }
  else
  {
    // initialize PID controller
    PID_setup();
  }

//outdated I use UART communication now
//  if (RunMode == DevMode)
//  {
//    if (servo_setup() == false)
//    {
//      Serial.println("servo_setup error!");
//      delay(100);
//    }
//  }
//  else
//  {
//    // initialize servo output for motor controller
//    servo_setup();
//  }

  if (RunMode == DevMode)
  {
    if (VESC_setup() == false)
    {
      Serial.println("VESC_setup error!");
      delay(100);
    }
  }
  else
  {
    // initialize UART output to the VESC for motor controller
    VESC_setup();
  }

  if (RunMode == DevMode)
  {
    if (adc_setup() == false)
    {
      Serial.println("adc_setup error!");
      delay(100);
    }
  }
  else
  {
    //initialise ADC
    adc_setup();
  }

  if (RunMode == DevMode)
  {
    if (safety_loop_setup() == false)
    {
      Serial.println("safety_loop_setup error!");
      delay(100);
    }
  }
  else
  {
    //initialize interrupts for safety_loop
    safety_loop_setup();
  }

  if (RunMode == DevMode)
  {
    if (steering_loop_setup() == false)
    {
      Serial.println("steering_loop_setup error!");
      delay(100);
    }
  }
  else
  {
    //initialize interrupts for steering_loop
    steering_loop_setup();
  }

  if (RunMode == DevMode)
  {
    Serial.println("setup ready");
    delay(100);
   
  }

  if (RunMode == DevMode)
  {
    if (control_loop_setup() == false)
    {
      Serial.println("control_loop_setup error!");
      delay(100);
    }
  }
  else
  {
    //initialize interrupts for control_loop
    control_loop_setup();
  }

  if (RunMode == DevMode)
  {
    if (userInterface_setup() == false)
    {
      Serial.println("userInterface_setup error!");
      delay(100);
    }
  }
  else
  {
    //initialize interrupts for userInterface_setup
    userInterface_setup();
  }

  interrupts();
  Serial.println("setup complete interrupts enabled");
}

void loop() {
  // put your main code here, to run repeatedly:

  #if defined DEBUG_MPU || DEBUG_PID || DEBUG_steering
  debug_serial_update();
  debug_display_update();
  #endif

}
