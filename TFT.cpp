#include <SPI.h>

#include "TFT.h"
#include "bitmap.h"
const int LEDPIN = 21;


Tft::Tft()
    : ledPin(LEDPIN), // Initialize ledPin in the initializer list
      screen(TFT_eSPI()), // Initialize screen in the initializer list
      ledOn(false)
      
{}

void Tft::setup() {
  pinMode(ledPin, OUTPUT);
  setLED(false);

  screen.init();   // initialize a ST7735S chip
  screen.fillScreen(TFT_BLACK);
}
void Tft::startSequence() {
    isInSequence = true;
    sequenceStartTime = millis();
    setLED(true);  // Start with eyes off
    displayBell(false);
    //testdrawtext("test: ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa, fringilla sed malesuada et, malesuada sit amet turpis. Sed porttitor neque ut ante pretium vitae malesuada nunc bibendum. Nullam aliquet ultrices massa eu hendrerit. Ut sed nisi lorem. In vestibulum purus a tortor imperdiet posuere. ", TFT_WHITE);
}
void Tft::update_time(){
      printTime(false);
    
}


void Tft::update() {
    if (!isInSequence) return;
    static bool mirrorflag = false;


    if (isInSequence){
    unsigned long currentTime = millis();
    if (currentTime - sequenceStartTime >= (3*TEXT_DURATION) / 4){
      displayBell(true);
    } else if (currentTime - sequenceStartTime >= (2*TEXT_DURATION) / 4){
      displayBell(false);
    } else if (currentTime - sequenceStartTime >= (TEXT_DURATION) / 4){
      displayBell(true);
    }




    
    // LEDs on
    if ( currentTime - sequenceStartTime >= TEXT_DURATION) {
        setLED(false);
        screen.fillScreen(TFT_BLACK);
        printTime(true);
        isInSequence = false;
    }
    }
}

void Tft::printTime(bool force){
  static int lastminute;
  //Serial.println(1);
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;

  }
  if (((timeinfo.tm_min) != lastminute) || force){
    if (!getLED()){
      setLED(true);
    }
    //Serial.println(2);
    screen.setTextSize(4);
    screen.setCursor(0, 45);
    //byte xpos = 6;
    //byte ypos = 0;
    //screen.drawString("88:88",xpos,ypos,7); // Overwrite the text to clear it
    screen.setTextColor(0xFBE0); // Orange
    //screen.printf();
    //screen.print("88:88");
    screen.fillScreen(TFT_BLACK);
    if(timeinfo.tm_hour < 10){
      screen.print(0);
    }
    screen.print(timeinfo.tm_hour);
    screen.print(":");
    if(timeinfo.tm_min < 10){
      screen.print(0);
    }
    screen.print(timeinfo.tm_min);
    lastminute = timeinfo.tm_min;
  }
}


void Tft::setLED(bool on) {
    digitalWrite(ledPin, on ? HIGH : LOW);
    ledOn = on;
}

bool Tft::getLED() {
    return digitalRead(ledPin);
}

bool Tft::isFinished(){
  return !isInSequence;
}

void Tft::testdrawtext(char *text, uint16_t color) {
  screen.fillScreen(TFT_BLACK);
  screen.setCursor(0, 0);
  screen.setTextColor(color);
  screen.setTextWrap(true);
  screen.print(text);
}
void Tft::displayBell(bool mirrored) {
  static bool mirrorflag = false;

  if (mirrored == mirrorflag){
    mirrorflag = !mirrorflag;
  
  screen.fillScreen(TFT_BLACK);
  screen.setCursor(0, 0);
  if (mirrored){
      screen.drawXBitmap(0, 0, bell2, logoWidth, logoHeight, TFT_WHITE, TFT_RED);
  } else{
      screen.drawXBitmap(0, 0, bell1, logoWidth, logoHeight, TFT_WHITE, TFT_RED);

  }
  }
}