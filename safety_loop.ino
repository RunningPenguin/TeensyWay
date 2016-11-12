bool safety_loop_setup()
{
  Serial.println("safety_loop_setup");
  delay(100);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  delay(100);
  colorWipe(strip.Color(0, 0, 255), 50);  // Blue
  delay(200);
  colorWipe(strip.Color(0, 0, 0), 50);    // Black/off
  delay(1000);

  task_low.begin(safety_loop, 200000);  //200000 safety_loop to run every 0.2 seconds 200ms
  task_low.priority(80);               // 80 should be capable to interrupt userInterface_loop
  
  return true;
}

long startup__delay_counter = 0;

void safety_loop()
{
  if (startup_ok == true)
  {
    check_run();
  }
  else
  {
    check_start();
    if (steering_status == true && pitch_status == true && roll_status == true)
    {
      startup_ok = true;
      all_ok();
    }
  }
}

boolean emergency()
{
  task_high.end();
  task_mid.end();
  task_low.end();
  
  power_left = 0;
  power_right = 0;
  servo_update();

  //all Neopixels red
  strip.setPixelColor(NEOPIXEL_FRONT, red);
  strip.setPixelColor(NEOPIXEL_RIGHT_FRONT, red);
  strip.setPixelColor(NEOPIXEL_RIGHT, red);
  strip.setPixelColor(NEOPIXEL_RIGHT_BACKWARD, red);
  strip.setPixelColor(NEOPIXEL_BACKWARD, red);
  strip.setPixelColor(NEOPIXEL_LEFT_BACKWARD, red);
  strip.setPixelColor(NEOPIXEL_LEFT, red);
  strip.setPixelColor(NEOPIXEL_LEFT_FRONT, red);
  strip.setPixelColor(NEOPIXEL_MIDDLE, red);
  strip.show();
}

boolean warning()
{
  //all Neopixels yellow
  strip.setPixelColor(NEOPIXEL_FRONT, yellow);
  strip.setPixelColor(NEOPIXEL_RIGHT_FRONT, yellow);
  strip.setPixelColor(NEOPIXEL_RIGHT, yellow);
  strip.setPixelColor(NEOPIXEL_RIGHT_BACKWARD, yellow);
  strip.setPixelColor(NEOPIXEL_BACKWARD, yellow);
  strip.setPixelColor(NEOPIXEL_LEFT_BACKWARD, yellow);
  strip.setPixelColor(NEOPIXEL_LEFT, yellow);
  strip.setPixelColor(NEOPIXEL_LEFT_FRONT, yellow);
  strip.setPixelColor(NEOPIXEL_MIDDLE, yellow);
  strip.show();
}

boolean all_ok()
{
  //all Neopixels black besides the middle one to green
  strip.setPixelColor(NEOPIXEL_FRONT, black);
  strip.setPixelColor(NEOPIXEL_RIGHT_FRONT, black);
  strip.setPixelColor(NEOPIXEL_RIGHT, black);
  strip.setPixelColor(NEOPIXEL_RIGHT_BACKWARD, black);
  strip.setPixelColor(NEOPIXEL_BACKWARD, black);
  strip.setPixelColor(NEOPIXEL_LEFT_BACKWARD, black);
  strip.setPixelColor(NEOPIXEL_LEFT, black);
  strip.setPixelColor(NEOPIXEL_LEFT_FRONT, black);
  strip.setPixelColor(NEOPIXEL_MIDDLE, green);
  strip.show();
}

boolean check_run()
{
  if (pitch >= MAX_PITCH || pitch <= -MAX_PITCH || roll >= MAX_ROLL || roll <= -MAX_ROLL) //used for determining if the platform is leveled (no startup with positive pitch angle)
  {
    emergency();
  }

  else if(pitch >= MAX_PITCH/2 && pitch < MAX_PITCH || pitch <= -MAX_PITCH/2 && pitch > -MAX_PITCH || roll >= MAX_ROLL/2 && roll < MAX_ROLL || roll <= -MAX_ROLL/2 && roll > -MAX_ROLL) //used for determining if the platform is leveled (no startup with positive pitch angle)
  {
    warning();
  }

  else if (pitch > STARTING_PITCH && pitch < MAX_PITCH/2+STARTING_PITCH || pitch < -STARTING_PITCH && pitch > -MAX_PITCH/2+STARTING_PITCH || roll > -MAX_ROLL && roll < 0 || roll < MAX_ROLL && roll > 0) //everything in range
  {
    all_ok();
  }
  
  VESC_update();  //TODO on this place because if the VESC updates every time a new PID value is calculated the Teensy hang up
  
}

boolean check_start()
{
  if(steering_status == false || pitch_status == false || roll_status == false)
  {
    if (check_steering_start() == true && check_pitch_start() == true && check_roll_start() == true)
    {
      startup__delay_counter++;

      if (startup__delay_counter >= 10)
      {
        steering_status = true;
        pitch_status = true;
        roll_status = true;
      }
    }
    else
    {
      startup__delay_counter = 0;
      steering_status = false;
      pitch_status = false;
      roll_status = false;
    }
  }
}

boolean check_steering_start()
{
  if (USE_STEERING == true)
  {
    if (steering_angle >= 2*STARTING_STEERING_ANGLE_RANGE) //used for determining if the steering column is in the middle (no startup with positive steering angle)
    {
      strip.setPixelColor(NEOPIXEL_RIGHT, red); //right Neopixel set to red
      strip.setPixelColor(NEOPIXEL_LEFT, black); //turn off the opposite Neopixel
      strip.show();
      return false;
    }
    else if (steering_angle <= -2*STARTING_STEERING_ANGLE_RANGE) //used for determining if the steering column is in the middle (no startup with negative steering angle)
    {
      strip.setPixelColor(NEOPIXEL_LEFT, red); //left Neopixel set to red
      strip.setPixelColor(NEOPIXEL_RIGHT, black); //turn off the opposite Neopixel
      strip.show();
      return false;
    }
    else if(steering_angle >= STARTING_STEERING_ANGLE_RANGE && steering_angle < 2*STARTING_STEERING_ANGLE_RANGE) //used for determining if the steering column is in the middle (no startup with positive steering angle)
    {
      strip.setPixelColor(NEOPIXEL_RIGHT, yellow); //right Neopixel set to yellow
      strip.setPixelColor(NEOPIXEL_LEFT, black); //turn off the opposite Neopixel
      strip.show();
      return false;
    }
    else if (steering_angle <= -STARTING_STEERING_ANGLE_RANGE && steering_angle > -2*STARTING_STEERING_ANGLE_RANGE) //used for determining if the steering column is in the middle (no startup with negative steering angle)
    {
      strip.setPixelColor(NEOPIXEL_LEFT, yellow); //left Neopixel set to yellow
      strip.setPixelColor(NEOPIXEL_RIGHT, black); //turn off the opposite Neopixel
      strip.show();
      return false;
    }
    else if (steering_angle > STARTING_STEERING_ANGLE && steering_angle < STARTING_STEERING_ANGLE_RANGE) //everything in range
    {
      strip.setPixelColor(NEOPIXEL_RIGHT, green); //right Neopixel set to green
      strip.setPixelColor(NEOPIXEL_LEFT, black); //turn off the opposite Neopixel
      strip.show();
      return true;
    }
    else if (steering_angle < -STARTING_STEERING_ANGLE && steering_angle > -STARTING_STEERING_ANGLE_RANGE) //everything in range
    {
      strip.setPixelColor(NEOPIXEL_LEFT, green); //left Neopixel set to green
      strip.setPixelColor(NEOPIXEL_RIGHT, black); //turn off the opposite Neopixel
      strip.show();
      return true;
    }
  }
  else if (USE_STEERING == false)
  {
    strip.setPixelColor(NEOPIXEL_LEFT, blue); //left Neopixel set to red
    strip.setPixelColor(NEOPIXEL_RIGHT, blue); //right Neopixel set to red
    strip.show();
    return true;
  }
}

boolean check_pitch_start()
{
  if (pitch >= 2*STARTING_PITCH_RANGE) //used for determining if the platform is leveled (no startup with positive pitch angle)
  {
    strip.setPixelColor(NEOPIXEL_FRONT, red); //front Neopixel set to red
    strip.setPixelColor(NEOPIXEL_BACKWARD, black); //turn off the opposite Neopixel
    strip.show();
    return false;
  }
  else if (pitch <= -2*STARTING_PITCH_RANGE) //used for determining if the platform is leveled (no startup with negative pitch angle)
  {
    strip.setPixelColor(NEOPIXEL_BACKWARD, red); //backward Neopixel set to red
    strip.setPixelColor(NEOPIXEL_FRONT, black); //turn off the opposite Neopixel
    strip.show();
    return false;
  }
  else if(pitch >= STARTING_PITCH_RANGE && pitch < 2*STARTING_PITCH_RANGE) //used for determining if the platform is leveled (no startup with positive pitch angle)
  {
    strip.setPixelColor(NEOPIXEL_FRONT, yellow); //front Neopixel set to yellow
    strip.setPixelColor(NEOPIXEL_BACKWARD, black); //turn off the opposite Neopixel
    strip.show();
    return false;
  }
  else if (pitch <= -STARTING_PITCH_RANGE && pitch > -2*STARTING_PITCH_RANGE) //used for determining if the platform is leveled (no startup with negative pitch angle)
  {
    strip.setPixelColor(NEOPIXEL_BACKWARD, yellow); //backward Neopixel set to yellow
    strip.setPixelColor(NEOPIXEL_FRONT, black); //turn off the opposite Neopixel
    strip.show();
    return false;
  }
  else if (pitch > STARTING_PITCH && pitch < STARTING_PITCH_RANGE) //everything in range
  {
    strip.setPixelColor(NEOPIXEL_FRONT, green); //front Neopixel set to green
    strip.setPixelColor(NEOPIXEL_BACKWARD, black); //turn off the opposite Neopixel
    strip.show();
    return true;
  }
  else if (pitch < -STARTING_PITCH && pitch > -STARTING_PITCH_RANGE) //everything in range
  {
    strip.setPixelColor(NEOPIXEL_BACKWARD, green); //front Neopixel set to green
    strip.setPixelColor(NEOPIXEL_FRONT, black); //turn off the opposite Neopixel
    strip.show();
    return true;
  }
  return false;
}

boolean check_roll_start()
{
  if (roll >= MAX_ROLL) //used for determining if the platform side-to-side roll from level ground is in a good starting range (no startup with too much left roll angle)
  {
    strip.setPixelColor(NEOPIXEL_RIGHT_FRONT, black); //turn off the opposite Neopixel
    strip.setPixelColor(NEOPIXEL_RIGHT_BACKWARD, black); //turn off the opposite Neopixel
    strip.setPixelColor(NEOPIXEL_LEFT_FRONT, yellow); //left-front Neopixel set to yellow
    strip.setPixelColor(NEOPIXEL_LEFT_BACKWARD, yellow); //left-backward Neopixel set to yellow
    strip.show();
    return false;
  }
  else if (roll <= -MAX_ROLL) //used for determining if the platform side-to-side roll from level ground is in a good starting range (no startup with too much right roll angle)
  {
    strip.setPixelColor(NEOPIXEL_RIGHT_FRONT, yellow); //right-front Neopixel set to yellow
    strip.setPixelColor(NEOPIXEL_RIGHT_BACKWARD, yellow); //right-backward Neopixel set to yellow
    strip.setPixelColor(NEOPIXEL_LEFT_FRONT, black); //turn off the opposite Neopixel
    strip.setPixelColor(NEOPIXEL_LEFT_BACKWARD, black); //turn off the opposite Neopixel
    strip.show();
    return false;
  }
  else if (roll > -MAX_ROLL && roll < 0) //everything in range
  {
    strip.setPixelColor(NEOPIXEL_RIGHT_FRONT, green); //right-front Neopixel set to green
    strip.setPixelColor(NEOPIXEL_RIGHT_BACKWARD, green); //right-backward Neopixel set to green
    strip.setPixelColor(NEOPIXEL_LEFT_FRONT, black); //turn off the opposite Neopixel
    strip.setPixelColor(NEOPIXEL_LEFT_BACKWARD, black); //turn off the opposite Neopixel
    strip.show();
    return true;
  }
  else if (roll < MAX_ROLL && roll > 0) //everything in range
  {
    strip.setPixelColor(NEOPIXEL_RIGHT_FRONT, black); //turn off the opposite Neopixel
    strip.setPixelColor(NEOPIXEL_RIGHT_BACKWARD, black); //turn off the opposite Neopixel
    strip.setPixelColor(NEOPIXEL_LEFT_FRONT, green); //left-front Neopixel set to green
    strip.setPixelColor(NEOPIXEL_LEFT_BACKWARD, green); //left-backward Neopixel set to green
    strip.show();
    return true;
  }
  else //nothing in range
  {
    strip.setPixelColor(NEOPIXEL_RIGHT_FRONT, red); //turn off the opposite Neopixel
    strip.setPixelColor(NEOPIXEL_RIGHT_BACKWARD, red); //turn off the opposite Neopixel
    strip.setPixelColor(NEOPIXEL_LEFT_FRONT, red); //left-front Neopixel set to green
    strip.setPixelColor(NEOPIXEL_LEFT_BACKWARD, red); //left-backward Neopixel set to green
    strip.show();
    return false;
  }
  return false;
}
