#include <Arduino.h>
#include <Robot.h>
#include <Vision.h>
#include <QTRSensors.h>
#include <Chassis.h>
#include <Rangefinder.h>

long duration;
double distance;
double Kp = 0.004;

double innerLeftLineTrackingValue;
double innerRightLineTrackingValue;
double outerLeftLineTrackingValue;
double outerRightLineTrackingValue;
double error;
double leftSpeed;
double rightSpeed;
int defaultSpeed = 75;


int trigPin = 12;
int echoPin = 17;

const uint8_t SensorCount = 4;
uint16_t sensorValues[SensorCount];

QTRSensors lineTrackers; 

Rangefinder rangefinder(echoPin, trigPin);


VisionSystem::VisionSystem()
{
}

void VisionSystem::setup()
{
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input

    // configure the sensors
    lineTrackers.setTypeAnalog();
    lineTrackers.setSensorPins((const uint8_t[]){19, 20, 22, 21}, SensorCount);

}

void VisionSystem::printDistance() {
    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance = duration * 0.034 / 2;
    // Prints the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.println(distance);
}

int VisionSystem::readDistance() {
    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance = duration * 0.034 / 2;
    
    return distance;
}

void VisionSystem::readLineTrackingSensor() {
    // read raw sensor values
    lineTrackers.read(sensorValues);

    // print the sensor values as numbers from 0 to 1023, where 0 means maximum
    // reflectance and 1023 means minimum reflectance
    for (uint8_t i = 0; i < SensorCount; i++)
    {
        Serial.print(sensorValues[i]);
        Serial.print('\t');
    }
    Serial.println();

    delay(250);
}

void VisionSystem::lineTrackDriving(Chassis chassis) {
    readLineTrackingSensor();
    outerRightLineTrackingValue = sensorValues[0] - 15;
    innerRightLineTrackingValue = sensorValues[1];
    innerLeftLineTrackingValue = sensorValues[2];
    outerLeftLineTrackingValue = sensorValues[3];

    //error = (leftLineTrackingValue - rightLineTrackingValue) * Kp;
    double error = ((outerLeftLineTrackingValue * -4) + (innerLeftLineTrackingValue * 0) + (innerRightLineTrackingValue * 0) + (outerRightLineTrackingValue * 4)) * Kp;
    //double sumOfReadings = sensorValues[0] + sensorValues[1] + sensorValues[2] + sensorValues[3];

    //error = error/sumOfReadings;


    Serial.print("error:  ");
    Serial.println(error);

    leftSpeed = defaultSpeed - error;
    rightSpeed = defaultSpeed + error;

    Serial.print("Left Speed:  ");
    Serial.println(leftSpeed);
    Serial.print("RightSpeed:  ");
    Serial.println(rightSpeed);
    chassis.setMotorEfforts(leftSpeed, rightSpeed);
}
void VisionSystem::inPlaceLineTracking(Chassis chassis) {
    readLineTrackingSensor();
    outerRightLineTrackingValue = sensorValues[0] - 15;
    innerRightLineTrackingValue = sensorValues[1];
    innerLeftLineTrackingValue = sensorValues[2];
    outerLeftLineTrackingValue = sensorValues[3];

    //error = (leftLineTrackingValue - rightLineTrackingValue) * Kp;
    double error = ((outerLeftLineTrackingValue * -4) + (innerLeftLineTrackingValue * 1) + (innerRightLineTrackingValue * 1) + (outerRightLineTrackingValue * 4)) * Kp;
    //double sumOfReadings = sensorValues[0] + sensorValues[1] + sensorValues[2] + sensorValues[3];

    //error = error/sumOfReadings;


    Serial.print("error:  ");
    Serial.println(error);

    leftSpeed = -error * 6;
    rightSpeed = error * 7.5;

    Serial.print("Left Speed:  ");
    Serial.println(leftSpeed);
    Serial.print("RightSpeed:  ");
    Serial.println(rightSpeed);
    chassis.setMotorEfforts(leftSpeed, rightSpeed);
}

void VisionSystem::ultrasonicLineTrackDriving(Chassis chassis, double targetDistance) {
    while(readDistance() > targetDistance) {
        lineTrackDriving(chassis);
        Serial.println("Driving");
    }
    chassis.setMotorEfforts(0, 0);
    Serial.println("Done");
}
void VisionSystem::intersectionLineTrackDriving(Chassis chassis) {
    outerRightLineTrackingValue = sensorValues[0] - 15;
    outerLeftLineTrackingValue = sensorValues[3];
    while(outerLeftLineTrackingValue < 450 && outerRightLineTrackingValue < 450) {
        outerRightLineTrackingValue = sensorValues[0] - 15;
        outerLeftLineTrackingValue = sensorValues[3];
        lineTrackDriving(chassis);
        Serial.println("Driving");
        Serial.println(outerRightLineTrackingValue);
        Serial.println(outerLeftLineTrackingValue);
    }
    chassis.setMotorEfforts(0, 0);
    //Serial.println("Finished");
    Serial.println(outerRightLineTrackingValue);
    //Serial.println(outerLeftLineTrackingValue);
}
void VisionSystem::ultrasonicDriving(Chassis chassis, double targetDistance) {
    while(readDistance() >= targetDistance) {
        chassis.setMotorEfforts(75, 75);
        Serial.println("Driving");
    }
    chassis.setMotorEfforts(0, 0);
    Serial.println("Done");
}

void VisionSystem::spinLineTracking(Chassis chassis) {
    readLineTrackingSensor();
    innerRightLineTrackingValue = sensorValues[1];
    innerLeftLineTrackingValue = sensorValues[2];

    //int i = 0;

    while (innerLeftLineTrackingValue < 450 && innerRightLineTrackingValue < 450) {
        chassis.setMotorEfforts(-60, 60);
        readLineTrackingSensor();
        innerRightLineTrackingValue = sensorValues[1];
        innerLeftLineTrackingValue = sensorValues[2];
        Serial.println("Not found");
        // if(innerLeftLineTrackingValue > 500 && innerRightLineTrackingValue > 500) {
        //     i++;
        //     delay(500);
        //     Serial.print("I:  ");
        //     Serial.println(i);
        // }
    }
    chassis.setMotorEfforts(0, 0);
    delay(500);
    Serial.println("Found");
}
void VisionSystem::spinLineTracking2(Chassis chassis) {
    readLineTrackingSensor();
    innerRightLineTrackingValue = sensorValues[1];
    innerLeftLineTrackingValue = sensorValues[2];

    //int i = 0;

    while (innerLeftLineTrackingValue < 500 && innerRightLineTrackingValue < 500) {
        chassis.setMotorEfforts(65, -65);
        readLineTrackingSensor();
        innerRightLineTrackingValue = sensorValues[1];
        innerLeftLineTrackingValue = sensorValues[2];
        Serial.println("Not found");
        // if(innerLeftLineTrackingValue > 500 && innerRightLineTrackingValue > 500) {
        //     i++;
        //     delay(500);
        //     Serial.print("I:  ");
        //     Serial.println(i);
        // }
    }
    chassis.setMotorEfforts(0, 0);
    delay(500);
    Serial.println("Found");
}

void VisionSystem::task3LineTracking(Chassis chassis) {
    readLineTrackingSensor();
    outerRightLineTrackingValue = sensorValues[0] - 15;
    innerRightLineTrackingValue = sensorValues[1];
    innerLeftLineTrackingValue = sensorValues[2];
    outerLeftLineTrackingValue = sensorValues[3];

    //int i = 0;

    while (outerRightLineTrackingValue < 500 && outerLeftLineTrackingValue < 500) {
        //lineTrackDriving(chassis);
        chassis.setMotorEfforts(100, 100);
        delay(1);
        readLineTrackingSensor();
        outerRightLineTrackingValue = sensorValues[0] - 15;
        innerRightLineTrackingValue = sensorValues[1];
        innerLeftLineTrackingValue = sensorValues[2];
        outerLeftLineTrackingValue = sensorValues[3];
        Serial.println("Not found");
        // if(innerLeftLineTrackingValue > 500 && innerRightLineTrackingValue > 500) {
        //     i++;
        //     delay(500);
        //     Serial.print("I:  ");
        //     Serial.println(i);
        // }
    }
    chassis.setMotorEfforts(0, 0);
    delay(500);
    Serial.println("Found");
}

bool VisionSystem::onLineTest(Chassis chassis) {
    outerRightLineTrackingValue = sensorValues[0] - 15;
    innerRightLineTrackingValue = sensorValues[1];
    innerLeftLineTrackingValue = sensorValues[2];
    outerLeftLineTrackingValue = sensorValues[3];

    if(innerLeftLineTrackingValue >= 450 && innerRightLineTrackingValue >= 450) {
        return true;
    }
    else {
        return false;
    }
}
bool VisionSystem::onLineTest2(Chassis chassis) {
    outerRightLineTrackingValue = sensorValues[0] - 15;
    innerRightLineTrackingValue = sensorValues[1];
    innerLeftLineTrackingValue = sensorValues[2];
    outerLeftLineTrackingValue = sensorValues[3];

    if(innerLeftLineTrackingValue > innerRightLineTrackingValue) {
        return true;
    }
    else {
        return false;
    }
}



