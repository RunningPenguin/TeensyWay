bool control_loop_setup()
{
  task_high.begin(control_loop, 80000);   //80000 control_loop to run every 0.08 seconds 80ms
  task_high.priority(64);                 //64 should be capable to interrupt userInterface_loop and steering_loop and safety_loop as well

  Serial.println("control_loop_setup");
  delay(100);
  
  return true;
}

void control_loop()
{
  if(startup_ok == true)
  {
    if (readCombinedYPR(ypr))
    {
      tiltPID.update();
      power_left = PID_power+steering_power;
      power_right = PID_power-steering_power;
//      servo_update(); //outdated I use UART communication now
//      VESC_update(); //TODO update moved to safety_loop because of suddenly hang up of the teensy if it resists here
    }
  }
  else
  {
    readCombinedYPR(ypr);
  }
  
}

