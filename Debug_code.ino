//#include <DogLcd.h>

// initialize the library with the numbers of the interface pins
//DogLcd LCD(DOGLCD_SI, DOGLCD_CLK, DOGLCD_RS, DOGLCD_CSB);

bool debug_setup()
{
  while(!Serial && (millis() <= 8000));                    // wait until serial ready
  Serial.begin(115200);
  Serial.println("Welcome to Droidway serial console");
  delay(1000);

  // set up the LCD type and the contrast setting for the display 
//  LCD.begin(DOG_LCD_M162);
  // Print a message to the LCD.
//  LCD.print("Welcome to Droidway");

  return true;
}

void debug_serial_update()
{
  #if defined DEBUG_MPU
  printYPR();
  #endif
  #if defined DEBUG_PID
  printPID();
  #endif
  #if defined DEBUG_steering
  printsteering();
  #endif
}

void debug_display_update()
{
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
//  LCD.setCursor(0, 1);
  // print the number of seconds since reset:
//  LCD.print(millis()/1000);
  
}

void printYPR()
{
  Serial.print("Yaw: ");
  Serial.print(yaw);
  Serial.print("\t\tPitch: ");
  Serial.print(pitch);
  Serial.print("\t\tRoll: ");
  Serial.print(roll);
  Serial.println();
}

void printPID()
{
  Serial.print("P: ");
  Serial.print(tiltPID.Pvalue);
  Serial.print("\t\tI: ");
  Serial.print(tiltPID.Ivalue);
  Serial.print("\t\tD: ");
  Serial.print(tiltPID.Dvalue);
  Serial.print("\t\tPID: ");
  Serial.print(PID_power);
  Serial.println();
}

void printsteering()
{
  Serial.print("raw steering_angle: ");
  Serial.print(Get_steering_angle());
  Serial.print("\tsteering_angle: ");
  Serial.print(steering_angle);
  Serial.print("\tsteeringsin: ");
  Serial.print(steeringsin);
  Serial.print("\tsteeringcos: ");
  Serial.print(steeringcos);
  Serial.print("\tSTEERING_OFFSET: ");
  Serial.print(STEERING_OFFSET);
  Serial.print("\tsteering_power: ");
  Serial.print(steering_power);
  Serial.print("\tPID: ");
  Serial.print(PID_power);
  Serial.println();
}

// Error codes
// 0 = IMU not detected or failed initialization (flashes "SOS": ...---...).
// 1 = initial IMU DMP memory load failed.
// 2 = DMP configuration updates failed.
// 3 = IMU FIFO overflow error. Code is too inefficient and IMU is not being read frequently enough.
// 4 = Maximum pitch or roll exceeded.
// 5 = Maximum power/speed exceeded.
// 6 = User reset.
void SOS(int error, char* details)
{

  //TODO Kommentierung aufheben, sodass bei reset alles aus geht.
  //servo1.writeMicroseconds(1500);
  //servo2.writeMicroseconds(1500);
  
//  tiltPID.reset();
//  tilt_power = -35; //to correct neutral position
//  setServos();

  power_left = 0; //PID_power+steering_power;
  power_right = 0;  //PID_power-steering_power;
  servo_update();
  VESC_update();
  
  pinMode(LED, OUTPUT);
  
  while (!digitalRead(RESET_PIN))
  {
    if (Serial) {
      Serial.print("error code: ");
      Serial.print(error);
      Serial.println(details);
    }
    
    if (error)
    {
      for (int i = 0; i < error; i++) {
        digitalWrite(LED, HIGH);
        delay(250);
        digitalWrite(LED, LOW);
        delay(250);
      }
    }
    else
    {
      // S
      for (int i = 0; i < 3; i++)
      {
        digitalWrite(LED, HIGH);
        delay(250);
        digitalWrite(LED, LOW);
        delay(250);
      }
      // O
      for (int i = 0; i < 3; i++)
      {
        digitalWrite(LED, HIGH);
        delay(500);
        digitalWrite(LED, LOW);
        delay(500);
      }
      // S
      for (int i = 0; i < 3; i++)
      {
        digitalWrite(LED, HIGH);
        delay(250);
        digitalWrite(LED, LOW);
        delay(250);
      }
    }
    
    if (Serial.available())
    {
      if (Serial.read() == 'r') { break;}
    }
    delay(1000);
  }
  digitalWrite(LED, LOW);
  
  while (digitalRead(RESET_PIN))
  {
    readDMP();
  }
  
//  checkToStart();
}
