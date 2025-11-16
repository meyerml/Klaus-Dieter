#include "LEDs.h"

void LEDs::open()
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
static void MP3Notify::OnError([[maybe_unused]] DfMp3& mp3, uint16_t errorCode)
{
  // see DfMp3_Error for code meaning
  Serial.println();
  Serial.print("Com Error ");
  Serial.println(errorCode);
}
static void MP3Notify::OnPlayFinished([[maybe_unused]] DfMp3& mp3, [[maybe_unused]] DfMp3_PlaySources source, uint16_t track)
{
  Serial.print("Play finished for #");
  Serial.println(track);  

  // start next track
  track += 1;
  // this example will just start back over with 1 after track 3
  if (track > 3) 
  {
    track = 1;
  }
  dfmp3.playMp3FolderTrack(track);  // sd:/mp3/0001.mp3, sd:/mp3/0002.mp3, sd:/mp3/0003.mp3
}
static void MP3Notify::OnPlaySourceOnline([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source)
{
  PrintlnSourceAction(source, "online");
}
static void MP3Notify::OnPlaySourceInserted([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source)
{
  PrintlnSourceAction(source, "inserted");
}
static void MP3Notify::OnPlaySourceRemoved([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source)
{
  PrintlnSourceAction(source, "removed");
}







// Constructor: Initialize serial on UART2, create MP3 player object
MP3Player::MP3Player() 
    : dfPlayerSerial(2),  // Use UART2
      dfMp3(dfPlayerSerial),  // Create MP3 player with this serial
      lastPlayTime(0),
      currentTrack(Ziege)
{}

void MP3Player::setup() {
    // Start serial communication
    Serial.begin(115200);
    
    // Initialize MP3 player on specific pins (adjust as needed)
    dfMp3.begin(25, 26);  // RX, TX pins
    
    // Reset to known state
    dfMp3.reset();
    
    // Set volume (0-30)
    dfMp3.setVolume(20);
    
    // Optionally, log some info
    Serial.println("MP3 Player initialized");
}

void MP3Player::update() {
    // Periodically play next track
    if (millis() - lastPlayTime > PLAY_INTERVAL) {
        // Cycle through tracks
        switch(currentTrack) {
            case Ziege:
                currentTrack = Alarm;
                break;
            case Alarm:
                currentTrack = Tor;
                break;
            case Tor:
                currentTrack = Ziege;
                break;
        }
        
        // Play the track
        playTrack(currentTrack);
        
        // Update last play time
        lastPlayTime = millis();
    }
    
    // Important: Allow MP3 player to process its internal events
    dfMp3.loop();
}

void MP3Player::playTrack(Tracks track) {
    Serial.print("Playing track: ");
    Serial.println(static_cast<int>(track));
    
    // Play track from MP3 folder
    dfMp3.playMp3FolderTrack(static_cast<uint16_t>(track));
}

void MP3Player::stop() {
    dfMp3.pause();
}