#ifndef SPEECH_H
#define SPEECH_H

#include <Arduino.h>

class Speech {
private:
    int randomTrack;

    bool isInSequence;
    int volume;


public:
    int numTracks;
    Speech();
    
    void setup();
    void update();
    
    // Control methods
    void startSequence();  // Start a 
    bool isFinished();
};











#endif
