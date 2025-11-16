#ifndef MP3_PLAYER_H
#define MP3_PLAYER_H

#include <Arduino.h>
#include <DFMiniMp3.h>
#include <HardwareSerial.h>

// Enum to define your tracks more cleanly
enum Tracks {
    Ziege = 1,  // corresponds to 0001.mp3
    Alarm = 2,  // corresponds to 0002.mp3
    Tor = 3     // corresponds to 0003.mp3
};

// forward declare the notify class, just the name
//
class Mp3Notify; 

// define a handy type using serial and our notify class
//
typedef DFMiniMp3<HardwareSerial, Mp3Notify> DfMp3; 

// implement a notification class,
// its member methods will get called 
//
class Mp3Notify
{
public:
  static void PrintlnSourceAction(DfMp3_PlaySources source, const char* action)
  static void OnError([[maybe_unused]] DfMp3& mp3, uint16_t errorCode)
  static void OnPlayFinished([[maybe_unused]] DfMp3& mp3, [[maybe_unused]] DfMp3_PlaySources source, uint16_t track)
  static void OnPlaySourceOnline([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source)
  static void OnPlaySourceInserted([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source)
  static void OnPlaySourceRemoved([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source)
};


class MP3Player {
private:
    // Hardware serial for communication
    HardwareSerial dfPlayerSerial;
    
    // The actual MP3 player object
    DfMp3 dfMp3;

    // Tracking playback state
    unsigned long lastPlayTime;
    const unsigned long PLAY_INTERVAL = 5000;  // 5 seconds between track changes
    Tracks currentTrack;



public:
    // Constructor
    MP3Player();

    // Standard Arduino-style methods
    void setup();
    void update();

    // Optional methods to control playback
    void playTrack(Tracks track);
    void stop();
};

#endif