#include <Arduino.h>
#include <Robot.h>
#include <Gripper.h>
#include <Vision.h>


Robot Romi;


void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  Romi.setup();
  delay(3000);
  
}


void loop() {
  // put your main code here, to run repeatedly:
  //Romi.testDrive();

}
