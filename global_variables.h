//Library for VESC UART
#include <VescUartControl.h>

// IMU variables and objects
#define MPU6050_DMP_FIFO_RATE ((200 / IMU_REFRESH_RATE) - 1)
MPU6050 imu;
// Places to store MPU6050 IMU readings
float ypr[3];      // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
float ypr_offset[3] = {0, STARTING_PITCH, 0}; // [yaw, pitch, roll] offset value container for manually adjusting zero angle
float yaw, pitch, roll; // Variables to hold adjusted values (ypr[] - ypr_offset[])

// PID variables and objects
float PID_power = 0;
float steering_power = 0;
float power_right = 0;
float power_left = 0;

float steering_percentage = 0;

bool steering_status = false;
bool pitch_status = false;
bool roll_status = false;
bool startup_ok = false;

float steering_angle = 0;

PID tiltPID = PID(&pitch, &PID_power, TILT_kP, TILT_kI, TILT_kD, TILT_DIRECTION); //TODO first argument was ypr[pitch]



struct bldcMeasure measuredVescRight;
struct bldcMeasure measuredVescLeft;

struct bldcLimits limitsVescRight;
struct bldcLimits limitsVescLeft;

bool VESC_right_measurements_ok;
bool VESC_left_measurements_ok;

bool VESC_right_limits_ok;
bool VESC_left_limits_ok;

float metersTravelled=0, kilometersTravelled=0;
float capacity_left=0;
