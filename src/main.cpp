#include <Arduino.h>
#include <Robot.h>
#include <Gripper.h>
#include <Vision.h>


Robot Romi;
GripperSystem Gripper;


void setup() {
  // put your setup code here, to run once:
  
}


void loop() {
  // put your main code here, to run repeatedly:
  Gripper.motorMoveTo(540);

}
