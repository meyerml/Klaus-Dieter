#ifndef TFT_H
#define TFT_H

#include <Arduino.h>
#include <TFT_eSPI.h> // Hardware-specific library

class Tft {
private:
    const int ledPin;
    TFT_eSPI screen;

    
    // Current state tracking
    bool ledOn;
    
    // Last time state was changed (for blinking etc.)
    unsigned long lastUpdateTime;

    // Sequence control
    bool isInSequence;
    unsigned long sequenceStartTime;
    
    // Timing constants
    static const unsigned long PATTERN_DURATION = 2000;    // Time eyes stay off during blink
    static const unsigned long TEXT_DURATION = 2000;    // Time between blink starts

public:
    Tft();
    
    void setup();
    void update();
    void update_time();
    
    // Control methods
    void startSequence();  // Start a 3-blink sequence
    void setLED(bool on);
    bool getLED();
    void testdrawtext(char *text, uint16_t color);
    bool isFinished();
    void printTime(bool force);
    void displayBell(bool mirrored);
};

#endif
