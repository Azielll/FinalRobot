#include <Arduino.h>
#include <Robot.h>
#include <Drive.h>
#include <Vision.h>
#include <Gripper.h>
#include <Chassis.h>


VisionSystem Vision;
GripperSystem Gripper;
Chassis Driver;

int gripperPosition1;
int gripperPosition2;


Robot::Robot()
{
}

void Robot::setup()
{

    Gripper.setup();
    Gripper.reset();
    Vision.setup();
}

void Robot::testGripperMotor() {
    Gripper.motorMoveToInDegrees(180);
    delay(2000);
    Gripper.motorMoveToInDegrees(0);
    delay(2000);

}



void Robot::testGripperServo() {
    Gripper.gripperServoRountine();
}

void Robot::gripperRoutine() {
    
}



