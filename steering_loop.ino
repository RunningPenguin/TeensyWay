bool steering_loop_setup()
{
  task_mid.begin(steering_loop, 100000); //100000 steering_loop to run every 0.1 seconds 100ms
  task_mid.priority(68);                //68 should be capable to interrupt userInterface_loop and safety_loop

  Serial.println("steering_loop_setup");
  delay(100);
  
  return true;
}

void steering_loop()
{
  if (USE_STEERING == true)
  {
    steering_percentage = (float)STEERING_RANGE/100;
    steering_angle = (((float)Get_steering_angle())-STEERING_OFFSET)/1000;
  
    steering_power = -steering_angle*steering_percentage*STEERING_GAIN;
  }
  else
  {
    steering_power = 0;
  }
}
