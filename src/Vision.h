#pragma once
#include <Chassis.h>


class VisionSystem {

    public: 
        VisionSystem();
        void setup();
        int readDistance();
        void printDistance();
        void readLineTrackingSensor();
        void lineTrackDriving(Chassis chassis);
        void inPlaceLineTracking(Chassis chassis);
        void ultrasonicDriving(Chassis chassis, double targetDistance);
        void ultrasonicLineTrackDriving(Chassis chassis, double targetDistance);
        void spinLineTracking(Chassis chassis);
        void spinLineTracking2(Chassis chassis);
        void task3LineTracking(Chassis chassis);
        void intersectionLineTrackDriving(Chassis chassis);
        bool onLineTest(Chassis chassis);
        bool onLineTest2(Chassis chassis);
        
        

    private:
        
        int innerLeftLineTracker = 20;
        int innerRightLineTracker = 22;
        int outerLeftLineTracker = 19;
        int outerRghtLineTracker = 21;
        
        
};