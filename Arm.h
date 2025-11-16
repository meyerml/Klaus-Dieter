#ifndef ARM_H
#define ARM_H

#include <Arduino.h>



class Arm {
private:
    const int leftPin;
    const int rightPin;
    //const int leftDistance;
    //const int rightDistance;
    bool isMoving;
    unsigned long moveStartTime;
    unsigned long moveDurationLeft;
    unsigned long moveDurationRight;
    unsigned long increment;
    bool isInSequence;
    unsigned long sequenceStartTime;
    static const unsigned long SEQUENCE_DURATION = 2000;  // Duration of the entire sequence

public:
    Arm();
    void moveLeft(unsigned long increment);
    void moveRight(unsigned long increment);

    void setup();
    void startSequence();
    void update();
    bool isFinished();
};

#endif
