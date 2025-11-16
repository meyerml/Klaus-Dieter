#ifndef TEETH_H
#define TEETH_H

#include <Arduino.h>
#include <ESP32Servo.h>



class Teeth {
private:
    Servo servo;
    const int servoPin;
    const int closedPosition1;
    const int closedPosition2;
    const int openPosition;
    bool isMoving;
    unsigned long moveStartTime;
    unsigned long moveDuration;
    bool isInSequence;
    int sequenceStep;
    unsigned long sequenceStartTime;
    static const unsigned long SEQUENCE_DURATION = 2000;  // Duration of the entire sequence

public:
    Teeth();

    void setup();
    void startSequence();
    void update();
    bool isFinished();

private:
    void moveToPosition(int position);
};

#endif
