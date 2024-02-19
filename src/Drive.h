#pragma once


class DriveSystem {

    public: 
        DriveSystem();
        void setup();
        void forward(int speed = 200);
        void stop();
        void backward();
        void left();
        void right();

    private:
        const int leftMotorPin = 16;
        const int rightMotorPin = 15;
        const int leftPWM = 10;
        const int rightPWM = 9;
        const int leftEncoder = 8;
        const int rightEncoder = 7;

};