// global variables
#import "global_variables.h"

Servo Motor_Right;
Servo Motor_Left;


bool servo_setup()
{
  Motor_Right.attach(SERVO_RIGHT);
  Motor_Right.writeMicroseconds(SERVO_NEUTRAL);  //after trying out the connection with VESC 4.7 and Firmware 2.18 zero rpm/current is at 1455µs not at 1500 like it should be
  Motor_Left.attach(SERVO_LEFT);
  Motor_Left.writeMicroseconds(SERVO_NEUTRAL);  //after trying out the connection with VESC 4.7 and Firmware 2.18 zero rpm/current is at 1455µs not at 1500 like it should be

  Serial.println("servo_setup");
  delay(100);

  return true;
}

void servoWrite(Servo servo, float value)
{
  servo.writeMicroseconds((int)map(value, -MAX_PID, MAX_PID, SERVO_NEUTRAL - SERVO_RANGE, SERVO_NEUTRAL + SERVO_RANGE)); //translates to "Motor_Right.writeMicroseconds(1000...2000)" or "Motor_Left.writeMicroseconds(1000...2000)"
}

void servo_update()
{
  servoWrite(Motor_Right, power_right);
  servoWrite(Motor_Left, power_left);
}
