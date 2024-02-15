#include <Arduino.h>
#include <Gripper.h>
#include <Romi32U4.h>

long oldValue = 0;
long newValue;
long count = 0;
unsigned time = 0;
long encoderPosition = 0;
const double p = 1.0;


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
}

long GripperSystem::motorGetPosition()
{
    long tempCount = 0;
    noInterrupts();
    tempCount = count;
    interrupts();
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
        setEffort(-effort, true);
    }
    else
    {
        setEffort(effort, false);
    }
}

void GripperSystem::setEffort(int effort, bool clockwise)
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
            //Serial.println(speed);
            motorSetEffort(speed - tolerance);
        }
    }
    else {

        while(count > targetPosition) {
            int speed = (count - targetPosition) * p;
            //Serial.println(speed);
            motorSetEffort(speed + tolerance);
        }
    }    
}
