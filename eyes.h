#ifndef EYES_H
#define EYES_H

#include <Arduino.h>

class Eyes {
private:
    const int leftEyePin;
    const int rightEyePin;
    
    // Current state tracking
    bool leftEyeOn;
    bool rightEyeOn;
    
    // Last time state was changed (for blinking etc.)
    unsigned long lastUpdateTime;

        // Sequence control
    bool isInSequence;
    int blinkCount;
    int totalBlinks = 3;
    bool eyesCurrentlyOn;
    unsigned long sequenceStartTime;
    unsigned long lastBlinkTime;
    
    // Timing constants
    static const unsigned long BLINK_DURATION = 200;    // Time eyes stay off during blink
    static const unsigned long BLINK_INTERVAL = 500;    // Time between blink starts

public:
    Eyes();
    
    void setup();
    void update();
    
    // Control methods
    void startSequence();  // Start a 3-blink sequence
    void blinkBoth();
    void wink(bool leftEye);  // true for left, false for right
    void setBoth(bool on);
    void setLeft(bool on);
    void setRight(bool on);
    bool isFinished();
};

#endif
