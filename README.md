# TeensyWay
Self balancing scooter controller with Teensy3.1

#Used hardware
- Teensy 3.1
- MPU6050 (inertial measurement unit. Used to detect the yaw, pitch and roll of the scooter)
- TLE5009 (MR based angular sensor. Used to detect the steering angle)
- VESC4.7 hardware with patched 2.18 firmware (see my other repository)
- 36V brushless hub motors

#Tested with the following tools
- Arduino 1.6.8
- Teensyduino 1.28

#the goals in my project at writing the balancing controller software were:
- reasonable partition of the software functions
- run every loop function in equidistant time slots
- make a hierarchy of every loop function so that the important functions could interrupt the unimportant ones
- read the configuration of the VESC at upstart so a change in the VESC (currents, limits) doesn´t require an update of my Teensy code
- only start the motors if everything is in neutral position
- show the upstart/neutral position so the driver could see what´s "wrong"
- provide an opportunity to enhance everything

#the actual working software concept
- one control loop - 80ms time slot - could interrupt all functions below in this list
- one steering loop - 100ms time slot - could interrupt all functions below in this list but not above
- one safety loop - 200ms time slot - could interrupt all functions below in this list but not above
- one user interface loop - 1s time slot - could interrupt all functions below in this list but not above

#the control loop should do the following work
- read yaw pitch roll
- check if the upstart is ok (is not true until the vehicle is in balancing position and without steering input)
- calculate the new currents (power) for the left and right wheel
- provides a PPM fallback if UART is disturbed

#the steering loop should do the tasks
- checks if steering is available/used
- reads the MR based angular sensor (analog sin/cos signal) for the steering information
- calculate the steering angle
- calculates the steering power (an offset which is added, subtracted to the control loop single wheel power)

#the safety loop should do the work
- check if the vehicle is in neutral position
- shows the upstart/neutral position through a ring WS2812 LEDs and a dot in the middle
- talks to the VESC over UART (reads the configuration and writes the currents) it´s currently in this loop because the UART is to slow to handle it in the control loop - original it should be there
- when the vehicle is running (brought into neutral position after starting) checks the pitch and roll if these are within the limits if outside shut off the balancing
- warn the driver before he gets into the shut off limits (different limits for warning and shut off)

#the user interface loop is in the actual version useless for driving because there is no user interface except the status LEDs
- reads the actual values of both VESCs
- calculates the traveled kilometers
- calculates the used capacity

#features I plan to integrate in the future
- add an 128x64 Display to show some information's like battery voltage, used capacity, traveled kilometers
- add warnings based on VESC status (erpm limit, battery cut off, speed, temperature, etc)
- add a few more ws2812 to show the battery status
- add an SD card to log everything
- add an Bluetooth module and program an android app to lock/unlock the vehicle and show these information's as well

#used libraries from the internet
- DebugUtils
- FreeIMU
- I2Cdev
- Kalman
- MPU6050
- PID
- VescUartControl (see my other repository)

I will take a look through all used libraries and provide links here in the future or if I made some changes push it to my account.
