#pragma once


class GripperSystem {

    public: 
        GripperSystem();
        void motorSetEffort(int effort);
        void motorSetEffort(int effort, bool clockwise);
        long motorGetPosition();
        void reset();
        void setup();
        double motorCalculateRPM(int ticks, int ticksPerRev);
        void motorMoveTo(int targetPosition);
        void motorMoveToInDegrees(double degrees);
        void calculatePosition();
        void openGripper();
        void closeGripper();
        void gripperServoRountine();


    private:
        static void isrA();
        static void isrB();
        const int tolerance = 52;
        const int PWMOutPin = 11;
        const int AIN2 = 13;
        const int AIN1 = 4;
        const int ENCA = 0;
        const int ENCB = 1;

};