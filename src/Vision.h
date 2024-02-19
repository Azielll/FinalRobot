#pragma once


class VisionSystem {

    public: 
        VisionSystem();
        void setup();
        int readDistance();
        void printDistance();

    private:
        int trigPin;
        int echoPin;


};