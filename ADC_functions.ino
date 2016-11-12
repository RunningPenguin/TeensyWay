ADC::Sync_result ADC_result;

uint16_t steeringsin = 0; //used
uint16_t steeringcos = 0; //used

bool adc_setup()
{
  // initialize ADC for steering sensor input
  pinMode(STEERING_SIN_PIN, INPUT);
  pinMode(STEERING_COS_PIN, INPUT);

  adc->setReference(ADC_REF_3V3, ADC_0);
  adc->setReference(ADC_REF_3V3, ADC_1);

  adc->setSamplingSpeed(ADC_VERY_HIGH_SPEED, ADC_0);     // change the sampling speed
  adc->setSamplingSpeed(ADC_VERY_HIGH_SPEED, ADC_1);     // change the sampling speed
  
  adc->setConversionSpeed(ADC_HIGH_SPEED_16BITS, ADC_0); // change the conversion speed
  adc->setConversionSpeed(ADC_HIGH_SPEED_16BITS, ADC_1); // change the conversion speed
  
  adc->setAveraging(6, ADC_0); // set number of averages
  adc->setAveraging(6, ADC_1); // set number of averages
  
  adc->setResolution(16, ADC_0); // set bits of resolution
  adc->setResolution(16, ADC_1); // set bits of resolution

  adc->startSynchronizedContinuous(STEERING_SIN_PIN, STEERING_COS_PIN);

  return true;
}

uint16_t Get_steering_angle(void)
{
  ADC_result = adc->readSynchronizedContinuous();
  // if using 16 bits and single-ended is necessary to typecast to unsigned,
  // otherwise values larger than 3.3/2 will be interpreted as negative
  ADC_result.result_adc0 = (uint16_t)ADC_result.result_adc0;
  ADC_result.result_adc1 = (uint16_t)ADC_result.result_adc1;

  steeringsin = ADC_result.result_adc0;
  steeringcos = ADC_result.result_adc1;

  /*
  steering_angle = (int16_t)(((atan2(steeringsin, steeringcos))*(180/PI))+angle_offset);
  if(steering_angle <= -angle_range)
  {
    steering_angle = -angle_range;
  }
  if(steering_angle >= angle_range)
  {
    steering_angle = angle_range;
  }
  */

  return ((atan2(steeringsin, steeringcos))*(180/PI))*1000;
}
