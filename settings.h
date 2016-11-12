#define RunMode DevMode
#define USE_STEERING true

//define if we are in development or normal mode
#define DevMode 1
#define NormalMode 2

//================= Hardware dependent Settings =================//
//PPM servo output pins to connect the motor controller
#define SERVO_RIGHT 20        // right motor control servo connection
#define SERVO_LEFT 21         // left motor control servo connection

// ADC pins A0 and A2 for synchronized conversation
#define STEERING_SIN_PIN A0   // pin connected to the sinus channel of the AMR/GMR sensor
#define STEERING_COS_PIN A2   // pin connected to the cosinus channel of the AMR/GMR sensor
#define STEERING_ANGLE_LEFT 47200 //21.1
#define STEERING_ANGLE_RIGHT 59200 //35.6
#define STEERING_RANGE 12000//max(STEERING_ANGLE_RIGHT,STEERING_ANGLE_LEFT)-min(STEERING_ANGLE_RIGHT,STEERING_ANGLE_LEFT)
#define STEERING_OFFSET 53200//min(STEERING_ANGLE_RIGHT,STEERING_ANGLE_LEFT)+(max(STEERING_ANGLE_RIGHT,STEERING_ANGLE_LEFT)-min(STEERING_ANGLE_RIGHT,STEERING_ANGLE_LEFT))/2 //with the calculation the right value is calculated but in sterring_loop there is a variable overflow
#define STEERING_GAIN 0.1

#define MPU6050_SCL 19        // pin SCL of MPU6050 IMU
#define MPU6050_SDA 18        // pin SDA of MPU6050 IMU

#define EMRGENCY_SWITCH 2     // pin connected to an normally closed (NC) switch which must be activated to arm the self balancing vehicle
#define LEFT_FEET_SWITCH
#define RIGHT_FEET_SWITCH

#define RESET_PIN 15

#define LED 13

#define DOGLCD_SI 5      // LCD SI pin to digital pin 5
#define DOGLCD_CLK 6     // LCD CLK pin to digital pin 6
#define DOGLCD_RS 7      // LCD RS pin to digital pin 7
#define DOGLCD_CSB 8     // LCD CSB pin to digital pin 8

#define NEOPIXEL_DATA_PIN 2

#define VOLTAGE_MEASUREMENT_HV A1
#define VOLTAGE_DIVIDER_HV 0.052

#define VOLTAGE_MEASUREMENT_LV A3
#define VOLTAGE_DIVIDER_LV 0.052
//=================================================================//

//================= Control and Behavior Settings =================//
#define TILT_DIRECTION FORWARD // Motor directions to balance. FORWARD/REVERSE reverse motor directions
#define TURN_DIRECTION FORWARD // Motor directions to turn. FORWARD/REVERSE reverse motor directions

/* Soft Control Limits
Exceeding these will cause the program to try to force slowing by overcompensating for inputs*/
#define POWER_LIMIT 0.75  // Percentage of MAX_POWER that causes overcompensation to force slowing

/* Hard Cutoff Limits
Exceeding these values will cause the program to stop and throw an error code*/
#define MAX_PITCH 35    // Maximum forward and backwards tilt from balanced position
#define MAX_ROLL 35     // Maximum side-to-side roll from level ground
#define MAX_POWER 0.85  // Percentage of maximum power allowed (ex: 0.90 = 90% of max power)

/* Startup Control Values
These settings control both initialization and restarting after an error*/
#define STARTING_STEERING_ANGLE 0.0        // Pitch angle required to start balancing
#define STARTING_STEERING_ANGLE_RANGE 0.5  // Pitch must be within STARTING_PITCH +-  STARTING_PITCH_RANGE
#define STARTING_PITCH 0.32        // Pitch angle required to start balancing defines also the pitch offset of the sensor
#define STARTING_PITCH_RANGE 2  // Pitch must be within STARTING_PITCH +-  STARTING_PITCH_RANGE
#define REQUIRED_TIME 500          // Milliseconds for which pitch must meet starting pitch and steering requirements
//=================================================================//

//================= IMU Settings =================//
/* IMU angle array positions. 
These are based on the Tilty pointing forward (USB away from the user
and being mounted upright (logo is visible from above) */
#define YAW 0
#define PITCH 2
#define ROLL 1

#define IMU_REFRESH_RATE 200   // IMU data refresh rate in Hz. Max of 200 hz, which may give noisy data.
                               // Should not go below 100 Hz for balancing
                               // Valid values: 200, 100, 67, 50, 40, 33, 29, 25

#define IMU_READ_ERROR_LIMIT 3 // Maximum consecutive IMU read errors before SOS is triggered
//================================================//



//================= PID Control Settings =================//
#define TILT_kP 10            // Proportional PID scalar. Scales with tilt.
#define TILT_kI 0           // Integral PID scalar. Scales with time and tilt.
#define TILT_kD 0            // Derivative PID scalar. Scales with rate of tilt.
#define MAX_PID SERVO_RANGE  // Maximum value allowed for P + I + D.
//========================================================//



//================= Servo Control Settings =================//
#define REFRESH_RATE 50 // Max refresh rate of the servo signals in Hz. THIS NUMBER CANNOT EXCEED 400Hz
                         // Standard R/C signal = 50Hz, 100 Hz usually works, >200 Hz is excessive
                         // A higher number means faster refresh and potentially smoother response, but
                         // some ESCs may not be able to handle higher speeds.

#define SERVO_RANGE 500  // Range (in us) to either side of neutral that high signal indicates full power.
                         // Servos usually use 1500 as neutral and 1000/2000 as min/max -> range = 500.

#define SERVO_NEUTRAL 1456
//==========================================================//

//============= VESC Motor Controller Settings =============//
#define MAX_AMP_MOTOR 0 //for debug only normally 30A ////DONE read configuration from VESC but for compatibility
//==========================================================//

//====================mechanical values====================//
#define CIRCUMFERENCE 131 // circumference of the wheels in cm
//=========================================================//

//=========================================================//
#define BATTERY_CAPACITY 7000 //Battery capacity in mAh
//=========================================================//

//======== Light indication of start o.k. settings ========//

#define NEOPIXEL_COUNT 9

#define NEOPIXEL_FRONT 4
#define NEOPIXEL_RIGHT 6
#define NEOPIXEL_BACKWARD 0
#define NEOPIXEL_LEFT 2

#define NEOPIXEL_LEFT_FRONT 3
#define NEOPIXEL_LEFT_BACKWARD 1

#define NEOPIXEL_RIGHT_FRONT 5
#define NEOPIXEL_RIGHT_BACKWARD 7

#define NEOPIXEL_MIDDLE 8
//==========================================================//
