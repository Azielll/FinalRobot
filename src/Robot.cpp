#include <Arduino.h>
#include <Robot.h>
#include <Vision.h>
#include <Gripper.h>
#include <Chassis.h>
#include <IRdecoder.h>
#include <ir_codes.h>

VisionSystem Vision;
GripperSystem Gripper;
Chassis chassis;

const uint8_t IR_DETECTOR_PIN = 14;
IRDecoder decoder(IR_DETECTOR_PIN);

enum State {IDLE, TASK1, TASK2, TASK3, TASK4, TASK5, STOP};
State currentState = IDLE;

enum PLAYPAUSE{YES, NO};
PLAYPAUSE playPauseState = NO;

int gripperPosition25 = 5050;
int gripperPosition45 = 4100;
int gripperPositionPlate = 350;

int house1Distance = 13;
int house2Distance = 10.50;


unsigned long actionStartTime = 0;
unsigned long interval = 20000;
unsigned long currentMillis = millis();

bool start = false; 

Robot::Robot()
{
    
}

void Robot::setup()
{

    chassis.init();
    decoder.init();
    Gripper.setup();
    Gripper.reset();
    Vision.setup();
}

void Robot::finalRoutine() {

    int code = decoder.getKeyCode();

    handleKeyPress(code);

    switch (currentState) {
    case IDLE:
        Serial.println("IDLE State");
        currentState = IDLE;

        break;
    case TASK1:
        Serial.println("Task1");
        task1();
        currentState = IDLE;
        break;
    case TASK2:
        Serial.println("Task2");
        task2();
        currentState = IDLE;
        break;
    case TASK3:
        Serial.println("Task3");
        // if(millis() - actionStartTime < interval) {
            
        // } 
        task3();
        currentState = IDLE;
        break;
    case TASK4:
        Serial.println("Task4");
        task4();
        currentState = IDLE;
        break;
    case TASK5:
        Serial.println("Task5");
        task5();
        currentState = IDLE;
        break;
    case STOP:
        State previousState = currentState;
        Serial.println("STOP MODE");
        if (playPauseState == YES) {
            chassis.setMotorEfforts(0, 0);
        }
        else {
            currentState = TASK3;
        }
        break;
  }
    
}

void Robot::handleKeyPress(int keyPress) {

    if (keyPress) {
        switch (keyPress) {
        case NUM_1: // Replace with your IR remote's LEFT button code
            currentState = TASK1;
            break;
        case NUM_2: // Replace with your IR remote's RIGHT button code
            currentState = TASK2;
            break;
        case NUM_3: // Replace with your IR remote's RIGHT button code
            currentState = TASK3;
            actionStartTime = millis();
            break;
        case NUM_4: // Replace with your IR remote's RIGHT button code
            currentState = TASK4;
            break;
        case NUM_5: // Replace with your IR remote's RIGHT button code
            currentState = TASK5;
            break;
        case PLAY_PAUSE: // Replace with your IR remote's RIGHT button code
            currentState = STOP;
            if (playPauseState == YES) {
                playPauseState = NO;
            }
            else {
                playPauseState = YES;
            }
            break;
        }
        
    }  
}

void Robot::task1() {
    Vision.ultrasonicDriving(chassis, house1Distance);
    gripper1Routine();
    Gripper.motorMoveTo(gripperPosition25 - 2000);
    chassis.driveFor(-10, 30);
    delay(1000);
    chassis.setMotorEfforts(-153, 153);
    delay(300);
    chassis.setMotorEfforts(0, 0);
    Vision.spinLineTracking(chassis);
    chassis.setMotorEfforts(-153, 153);
    delay(200);
    chassis.setMotorEfforts(0, 0);
    Vision.spinLineTracking(chassis);
    delay(1000);


    chassis.driveFor(-5.5, 20);
    delay(1000);
    //Vision.spinLineTracking2(chassis);
    //Gripper.motorMoveTo(gripperPositionPlate + 1000);
    //chassis.driveFor(2, 20);
    //chassis.driveFor(2, 20);
    Gripper.motorMoveTo(gripperPositionPlate + 500);
    //chassis.driveFor(4, 20);
    Gripper.motorMoveTo(gripperPositionPlate);
    Gripper.openGripper();
    delay(2000);
    Gripper.motorMoveTo(gripperPositionPlate + 200);
    // Gripper.closeGripper();
    // delay(2000);
    chassis.driveFor(-10, 10);  
    //Gripper.closeGripper();  
    
}
void Robot::task2() {
    Gripper.openGripper();
    Gripper.motorMoveTo(gripperPositionPlate);
    chassis.driveFor(7, 10); 
    Gripper.closeGripper();
    delay(2000);
    chassis.turnFor(178, 50);
     delay(5000); 
    //Vision.ultrasonicDriving(chassis, house1Distance + 4);
    //Gripper.motorMoveTo(gripperPosition25 + 700);
    
    chassis.driveFor(-8, 20);

    Gripper.motorMoveTo(gripperPosition25 + -1500);
    delay(1000);
    
    //Vision.spinLineTracking2(chassis);
    //chassis.setMotorEfforts(110, 110);
    //Vision.lineTrackDriving(chassis);
    chassis.setMotorEfforts(100, 100);
    delay(2900);
    chassis.setMotorEfforts(0, 0);
    Gripper.motorMoveTo(gripperPosition25 + 1000);
    Gripper.openGripper();
    chassis.driveFor(-11, 10);
    Gripper.motorMoveTo(gripperPositionPlate);
}
void Robot::task3() {
    
        // chassis.setMotorEfforts(100, 100);
        chassis.setMotorEfforts(130, -130);
        delay(300);
        chassis.setMotorEfforts(0, 0);
        Vision.spinLineTracking2(chassis);
        if(!(Vision.onLineTest(chassis))) {
            chassis.setMotorEfforts(0,0);
            delay(200);
            chassis.setMotorEfforts(0, 0);
        }
        Vision.ultrasonicLineTrackDriving(chassis, house2Distance);
        delay(5000);
        chassis.setMotorEfforts(0, 0);
        Vision.spinLineTracking(chassis);
        chassis.setMotorEfforts(-130, 130);
        delay(300);
        chassis.setMotorEfforts(0, 0);
        chassis.setMotorEfforts(53, -53);
        delay(4200);
       
     if(!(Vision.onLineTest(chassis))) {
        if(Vision.onLineTest2(chassis)) {
            chassis.setMotorEfforts(-100, 100);
            delay(200);
            chassis.setMotorEfforts(0, 0);
        }
        else{
            chassis.setMotorEfforts(100, -100);
            delay(200);
            chassis.setMotorEfforts(0, 0);
        }
     }

    
    // chassis.setMotorEfforts(-130, 130);
    // delay(300);
    // chassis.setMotorEfforts(0, 0);
    //  Vision.spinLineTracking(chassis);
    //  chassis.setMotorEfforts(-130, 130);
    //  delay(300);
    //  chassis.setMotorEfforts(0, 0);
    //  Vision.spinLineTracking(chassis);
    //  chassis.setMotorEfforts(100, 100);
    //  delay(6000);
}
void Robot::task4() {
    Vision.ultrasonicLineTrackDriving(chassis, house2Distance);
    gripper2Routine();
    //chassis.driveFor(-2, 20);
    //ripper.motorMoveTo(gripperPosition45 - 1200);
    // chassis.setMotorEfforts(-100, -100);
    // delay(1500);
    Gripper.motorMoveTo(gripperPosition45 + 1100);
    chassis.driveFor(-9, 15);
    delay(2000);
    chassis.setMotorEfforts(-100, 100);
    delay(400);
    chassis.driveFor(-2, 15);
    delay(2000);
    chassis.driveFor(-7, 15);
    delay(2000);
    chassis.setMotorEfforts(0, 0);
    Gripper.motorMoveTo(gripperPosition45 - 1200);
    chassis.setMotorEfforts(0, 0);
    chassis.setMotorEfforts(-100, 100);
    delay(400);
    chassis.setMotorEfforts(0, 0);
    Vision.spinLineTracking(chassis);
    chassis.setMotorEfforts(-100, 100);
    delay(400);
    chassis.setMotorEfforts(0, 0);
    Vision.spinLineTracking(chassis);
    delay(1000);
    // if(!(Vision.onLineTest(chassis))) {
    //     if(Vision.onLineTest2(chassis)) {
    //         chassis.setMotorEfforts(-100, 100);
    //         delay(200);
    //         chassis.setMotorEfforts(0, 0);
    //     }
    //     else{
    //         chassis.setMotorEfforts(100, -100);
    //         delay(200);
    //         chassis.setMotorEfforts(0, 0);
    //     }
    //  }
    //chassis.driveFor(-5.5, 20);
    delay(1000);
    chassis.driveFor(-5, 10);
    Gripper.motorMoveTo(gripperPositionPlate);
    //Gripper.motorMoveTo(gripperPositionPlate + 1000);
    //chassis.driveFor(2, 20);
    //chassis.driveFor(2, 20);
    //delay(1000);
    //chassis.setMotorEfforts(0, 0);
    Gripper.motorMoveTo(gripperPositionPlate - 200);
    Gripper.openGripper();
    delay(2000);
    Gripper.motorMoveTo(gripperPositionPlate + 200);
    // Gripper.closeGripper();
    // delay(2000);
    chassis.driveFor(-10, 10);
    delay(2000);
}
void Robot::task5() {
    Gripper.openGripper();
    Gripper.motorMoveTo(gripperPositionPlate + 100);
    chassis.driveFor(10, 10); 
    Gripper.closeGripper();
    delay(2000);
    Gripper.motorMoveTo(gripperPosition25 + 1000);
    delay(1000);
    chassis.driveFor(2, 20);
    delay(1000);
    chassis.turnFor(175, 50);
    delay(5000); 

    Gripper.motorMoveTo(gripperPosition25 - 2000);
    delay(1000);
    // chassis.setMotorEfforts(110, 110);
    //Vision.lineTrackDriving(chassis);
    //delay(8000);
    //Vision.lineTrackDriving(chassis);
    // delay(4500);
    chassis.driveFor(8, 30);
    delay(2000);
    chassis.setMotorEfforts(0, 0);
    Gripper.motorMoveTo(gripperPosition25 + 1000);
    chassis.driveFor(4.5, 30);
    delay(2000);
    Gripper.openGripper();
    Gripper.motorMoveTo(gripperPosition25 );
    delay(2000);
    chassis.driveFor(-7, 10);
    delay(2000);
    
    

}

void Robot::testGripperMotor() {
    //Gripper.closeGripper();
    //Gripper.openGripper();
    Gripper.motorGetPosition();
    //delay(2000);
    // Gripper.motorMoveTo(gripperPosition2);
    // Gripper.motorGetPosition();
    // delay(2000);
    
}

void Robot::testGripperServo() {
    Gripper.closeGripper();
}

void Robot::gripper1Routine() {
    Gripper.openGripper();
    Gripper.motorMoveTo(gripperPosition25);
    Gripper.motorGetPosition();
    delay(2000);
    //chassis.driveFor(6, 10);
    chassis.setMotorEfforts(95, 95);
    delay(700);
    chassis.setMotorEfforts(0, 0);
    Gripper.closeGripper();
    delay(3000);
    chassis.driveFor(1, 30);
    delay(1000);

}

void Robot::gripper2Routine() {
    Gripper.openGripper();
    Gripper.motorMoveTo(gripperPosition45 - 100);
    Gripper.motorGetPosition();
    delay(3000);
    //chassis.driveFor(6, 10);
    Gripper.closeGripper();
    delay(1000);
    chassis.setMotorEfforts(100, 100);
    delay(400);
    chassis.setMotorEfforts(0, 0);
    delay(100);
}

void Robot::testDrive() {
    //chassis.turnFor(180, 80);
    chassis.turnFor(-180, 80);
    delay(5000);
}

void Robot::testLineTrackers() {
    Vision.ultrasonicLineTrackDriving(chassis, house2Distance);
    //Vision.spinLineTracking(chassis);
    
}

void Robot::testIRRemote() {
    int code = decoder.getKeyCode();
    if(code >= 0) {
        Serial.print("Code:   ");
        Serial.println(code);
    }
}

void Robot::testUltrasonicSensor() {
    Vision.printDistance();
}




