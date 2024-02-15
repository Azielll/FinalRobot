#pragma once


class GripperSystem {

    public: 
        GripperSystem();
        void motorSetEffort(int effort);
        long motorGetPosition();
        void reset();
        void setup();
        double motorCalculateRPM(int ticks, int ticksPerRev);
        void motorMoveTo(int targetPosition);
        void calculatePosition();


    private:
        void setEffort(int effort, bool clockwise);
        static void isrA();
        static void isrB();
        const int tolerance = 52;
        const int PWMOutPin = 11;
        const int AIN2 = 4;
        const int AIN1 = 13;
        const int ENCA = 0;
        const int ENCB = 1;

};