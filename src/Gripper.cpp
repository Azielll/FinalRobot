#include <Arduino.h>
#include <Gripper.h>
#include <Romi32U4.h>
#include <Timer.h>
#include <servo32u4.h>


long oldValue = 0;
long newValue;
long count = 0;
unsigned time = 0;
long encoderPosition = 0;
const double p = 1.0;
const double ticksPerRevolution = 540;

int servoPin = 5;
int linearPotPin = 18;
int servoStop = 1490;  
int servoJawDown = 1300;  
int servoJawUp = 1700;  
int printDelay = 500;
int linearPotVoltageADC = 500;

int jawOpenPotVoltageADC = 780;
int jawClosedPotVoltageADC = 60;
int linearPotVoltageCheck = 0;
int linearPotVoltageDelta = 0;
int i = 0;
int c = 0;
boolean notPlate = false;


Romi32U4ButtonB buttonB;
Servo32U4 jawServo;
Timer printTimer(printDelay);


GripperSystem::GripperSystem()
{
}

void GripperSystem::setup()
{
    pinMode(PWMOutPin, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(AIN1, OUTPUT);
    pinMode(ENCA, INPUT);
    pinMode(ENCB, INPUT);
    TCCR1A = 0xA8; //0b10101000; //gcl: added OCR1C for adding a third PWM on pin 11
    TCCR1B = 0x11; //0b00010001;
    ICR1 = 400;
    OCR1C = 0;

    attachInterrupt(digitalPinToInterrupt(ENCA), isrA, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCB), isrB, CHANGE);
    reset();

    jawServo.attach();

}

long GripperSystem::motorGetPosition()
{
    long tempCount = 0;
    noInterrupts();
    tempCount = count;
    interrupts();
    Serial.print("Position:   ");
    Serial.println(tempCount);
    return tempCount;
}

void GripperSystem::reset()
{
    noInterrupts();
    count = 0;
    interrupts();
}


void GripperSystem::isrA()
{
    if (digitalRead(0) != digitalRead(1)) {
        count++;
    }
    else {
        count--;
    }
    
}

void GripperSystem::isrB()
{
    if (digitalRead(1) != digitalRead(0)) {
        count--;
    }
    else {
        count++;
    }
}

void GripperSystem::motorSetEffort(int effort)
{
    if (effort < 0)
    {
        motorSetEffort(-effort, true);
    }
    else
    {
        motorSetEffort(effort, false);
    }
}

void GripperSystem::motorSetEffort(int effort, bool clockwise)
{
    if (clockwise)
    {
        digitalWrite(AIN1, HIGH);
        digitalWrite(AIN2, LOW);
    }
    else
    {
        digitalWrite(AIN1, LOW);
        digitalWrite(AIN2, HIGH);
    }
    OCR1C = constrain(effort, 0, 400);
}

double GripperSystem::motorCalculateRPM(int ticks, int ticksPerRev) {

    return ((ticks / ticksPerRev) / 100); 
    
}

void GripperSystem::motorMoveTo(int targetPosition) {
    
    if (targetPosition > 0) {
        while(count < targetPosition) {
            int speed = (count - targetPosition) * p;
            Serial.println(speed);
            motorSetEffort(speed - tolerance);
        }
    }
    else {

        while(count > targetPosition) {
            int speed = (count - targetPosition) * p;
            Serial.println(speed);
            motorSetEffort(speed + tolerance);
        }
    }    
}

void GripperSystem::motorMoveToInDegrees(double degrees) {
    
    double targetPosition = (degrees/360.0) * ticksPerRevolution;
    Serial.println(targetPosition);

    if (targetPosition > 0) {
        while(count < targetPosition) {
            int speed = (count - targetPosition) * p;
            Serial.println(speed);
            motorSetEffort(speed - tolerance);
        }
    }
    else {

        while(count > targetPosition) {
            int speed = (count - targetPosition) * p;
            Serial.println(speed);
            motorSetEffort(speed + tolerance);
        }
    }    
}

void GripperSystem::openGripper() {
    // Move Jaw Down
    jawServo.writeMicroseconds(servoJawDown);

    while (linearPotVoltageADC < jawOpenPotVoltageADC)
    {
      linearPotVoltageADC = analogRead(linearPotPin);
      if (printTimer.isExpired()){
        Serial.print("linearPotVoltageADC:    ");
        Serial.println(linearPotVoltageADC);
      }
    }

    // Stop servo
    jawServo.writeMicroseconds(servoStop);
}

void GripperSystem::closeGripper() {
    
    linearPotVoltageCheck = linearPotVoltageADC;

    // Move Jaw Up
    jawServo.writeMicroseconds(servoJawUp);

    while (linearPotVoltageADC > jawClosedPotVoltageADC)
    {
      
        linearPotVoltageADC = analogRead(linearPotPin);
        if (i == 2) { 
            linearPotVoltageDelta = linearPotVoltageCheck - analogRead(linearPotPin);
            linearPotVoltageCheck = analogRead(linearPotPin);
            i = 0;
        }
        if (printTimer.isExpired()){
        
            Serial.print("linearPotVoltageADC:     ");
            Serial.println(linearPotVoltageADC);
            Serial.print("linearPotVoltageDelta:     ");
            Serial.println(linearPotVoltageDelta);
            i++;
            c++;
        }
        if (c >= 6 && linearPotVoltageDelta <= 20) {
            c =  0;
            jawServo.writeMicroseconds(servoStop);
            Serial.println("Obstruction found; re-opening...");
            delay(1000);
            jawServo.writeMicroseconds(servoJawDown);
            delay(4000);
            break;
        }
    }
    
    // Stop servo
    jawServo.writeMicroseconds(servoStop);
}

void GripperSystem::gripperServoRountine() {

    // Stop servo
    jawServo.writeMicroseconds(servoStop);
    delay(2000);
    // Get Pot Value
    linearPotVoltageADC = analogRead(linearPotPin);
    Serial.print("Initial linearPotVoltageADC:   ");
    Serial.println(linearPotVoltageADC);

    while(buttonB.isPressed()) {

        openGripper();

        // Stop servo
        jawServo.writeMicroseconds(servoStop);

        linearPotVoltageADC = analogRead(linearPotPin);
        Serial.print("Bottom linearPotVoltageADC Before Delay:    ");
        Serial.println(linearPotVoltageADC);
        delay(5000);
        linearPotVoltageADC = analogRead(linearPotPin);
        Serial.print("Bottom linearPotVoltageADC After Delay:     ");
        Serial.println(linearPotVoltageADC);
        delay(5000);

        closeGripper();

        // Stop servo
        jawServo.writeMicroseconds(servoStop);

        linearPotVoltageADC = analogRead(linearPotPin);
        Serial.print("Final linearPotVoltageADC Before Delay:      ");
        Serial.println(linearPotVoltageADC);
        delay(5000);
        linearPotVoltageADC = analogRead(linearPotPin);
        Serial.print("Final linearPotVoltageADC After Delay:      ");
        Serial.println(linearPotVoltageADC);
        delay(5000);

    }

    jawServo.writeMicroseconds(servoStop);

}