#include <Arduino.h>
#include <Robot.h>
#include <Gripper.h>
#include <Vision.h>
#include <Chassis.h>
#include "Timer.h"



Robot Romi;
//Chassis chassis;


void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  Romi.setup();
  delay(3000);
  
}


void loop() {
  // put your main code here, to run repeatedly:
  Romi.finalRoutine();
  
}
