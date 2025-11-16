
#include "Speech.h"
//enum tracks {Ziege, Alarm, Tor};
// this example will play a track and then 
// every five seconds play another track
//
// it expects the sd card to contain these three mp3 files
// but doesn't care whats in them
//
// sd:/mp3/0001.mp3
// sd:/mp3/0002.mp3
// sd:/mp3/0003.mp3

#include <DFMiniMp3.h>
#include <HardwareSerial.h>

const int VOLUME = 15;


HardwareSerial DFPlayerSerial(2);
//
class Mp3Notify; 

// define a handy type using serial and our notify class
//
typedef DFMiniMp3<HardwareSerial, Mp3Notify> DfMp3; 



// instance a DfMp3 object, 
//
DfMp3 dfmp3(DFPlayerSerial);

class Mp3Notify
{
public:
  static void PrintlnSourceAction(DfMp3_PlaySources source, const char* action)
  {
    if (source & DfMp3_PlaySources_Sd) 
    {
        Serial.print("SD Card, ");
    }
    if (source & DfMp3_PlaySources_Usb) 
    {
        Serial.print("USB Disk, ");
    }
    if (source & DfMp3_PlaySources_Flash) 
    {
        Serial.print("Flash, ");
    }
    Serial.println(action);
  }
  static void OnError([[maybe_unused]] DfMp3& mp3, uint16_t errorCode)
  {
    // see DfMp3_Error for code meaning
    Serial.println();
    Serial.print("Com Error ");
    Serial.println(errorCode);
  }
  static void OnPlayFinished([[maybe_unused]] DfMp3& mp3, [[maybe_unused]] DfMp3_PlaySources source, uint16_t track)
  {
   /* Serial.print("Play finished for #");
    Serial.println(track);  

    // start next track
    track += 1;
    // this example will just start back over with 1 after track 3
    if (track > 3) 
    {
      track = 1;
    }
    dfmp3.playMp3FolderTrack(track);  // sd:/mp3/0001.mp3, sd:/mp3/0002.mp3, sd:/mp3/0003.mp3
    */
  }
  static void OnPlaySourceOnline([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source)
  {
    PrintlnSourceAction(source, "online");
  }
  static void OnPlaySourceInserted([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source)
  {
    PrintlnSourceAction(source, "inserted");
  }
  static void OnPlaySourceRemoved([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source)
  {
    PrintlnSourceAction(source, "removed");
  }
};


Speech::Speech()
    : randomTrack(1),
      isInSequence(false),
      numTracks(1),
      volume(VOLUME)
{}

void Speech::setup() {
   dfmp3.begin(25, 26); // RX, TX

  // during development, it's a good practice to put the module
  // into a known state by calling reset().  
  // You may hear popping when starting and you can remove this 
  // call to reset() once your project is finalized
  dfmp3.reset();
  dfmp3.setVolume(volume);
  numTracks = dfmp3.getTotalTrackCount(DfMp3_PlaySource_Sd);
  //Serial.println(numTracks);
  randomSeed(micros());


}
void Speech::startSequence() {
    //static int track = 1;
    isInSequence = true;
    //Serial.println(randomTrack)
    // Generate a random integer between 0 and 99
    int randomTrack = random(1, numTracks+1);
    //randomTrack++;
    //int randomTrack = 2;
    //Serial.println(numTracks);
    //Serial.println(randomTrack);
    dfmp3.playMp3FolderTrack(randomTrack);  // sd:/mp3/0001.mp3
    isInSequence = false;
}

void Speech::update() {
}


bool Speech::isFinished(){
  return !isInSequence;
}
