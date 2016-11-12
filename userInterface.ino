float centimetersMovedRight=0, metersMovedRight=0, kilometersMovedRight=0;
float centimetersMovedLeft=0, metersMovedLeft=0, kilometersMovedLeft=0;
float capacity_used=0, capacity_regenerated=0;


bool userInterface_setup()
{
  task_very_low.begin(userInterface_loop, 1000000);    //userInterface_loop every 1s
  task_very_low.priority(88);                       // priority very low should be interrupted by almost everything

  Serial.println("userInterface_setup");
  delay(100);
  
  return true;
}

void userInterface_loop()
{
  if (VESC_read() == true)
  {
    centimetersMovedRight=(measuredVescRight.tachometerAbs/138)*CIRCUMFERENCE;
    metersMovedRight=centimetersMovedRight/100;
    kilometersMovedRight=metersMovedRight/1000;
  
    centimetersMovedLeft=(measuredVescLeft.tachometerAbs/138)*CIRCUMFERENCE;
    metersMovedLeft=centimetersMovedLeft/100;
    kilometersMovedLeft=metersMovedLeft/1000;

    if(VESC_right_measurements_ok ==1 && VESC_left_measurements_ok == 1)
    {
      metersTravelled=(metersMovedRight+metersMovedLeft)/2;
      kilometersTravelled=(kilometersMovedRight+kilometersMovedLeft)/2;

      capacity_used=measuredVescRight.ampHours*1000+measuredVescLeft.ampHours*1000;
      capacity_regenerated=measuredVescRight.ampHoursCharged*1000+measuredVescLeft.ampHoursCharged*1000;
      capacity_left=BATTERY_CAPACITY-capacity_used+capacity_regenerated;
    }
  
    if (RunMode == DevMode)
    {
      Serial.print("meters_Right: "); Serial.println(metersMovedRight);
      Serial.print("kilometers_Right: "); Serial.println(kilometersMovedRight);
      Serial.print("meters_Left: "); Serial.println(metersMovedLeft);
      Serial.print("kilometers_Left: "); Serial.println(kilometersMovedLeft);
      Serial.print("meters: "); Serial.println(metersTravelled);
      Serial.print("kilometers: "); Serial.println(kilometersTravelled);
      delay(100);
    }
  }
}
