#include <Arduino.h>
#include <Robot.h>
#include <Drive.h>


DriveSystem::DriveSystem()
{
}

void DriveSystem::setup()
{

    pinMode(leftMotorPin, OUTPUT);
    pinMode(rightMotorPin, OUTPUT);
    pinMode(leftPWM, OUTPUT);
    pinMode(rightPWM, OUTPUT);
    pinMode(leftEncoder, INPUT);
    pinMode(rightEncoder, INPUT);

    digitalWrite(leftMotorPin, LOW);
    digitalWrite(rightMotorPin, LOW);

    
}

void DriveSystem::forward(int speed = 200) {

    digitalWrite(leftMotorPin, HIGH);
    digitalWrite(rightMotorPin, HIGH);
    analogWrite(leftPWM, speed);
    analogWrite(rightPWM, speed);

}
void DriveSystem::stop() {

    digitalWrite(leftMotorPin, LOW);
    digitalWrite(rightMotorPin, LOW);
    analogWrite(leftPWM, 0);
    analogWrite(rightPWM, 0);
}