#include "Eyes.h"

const int LEFT_EYE_PIN = 15;
const int RIGHT_EYE_PIN = 5;

Eyes::Eyes()
    : leftEyePin(LEFT_EYE_PIN),
      rightEyePin(RIGHT_EYE_PIN),
      leftEyeOn(false),
      rightEyeOn(false),
      lastUpdateTime(0)
{}

void Eyes::setup() {
    pinMode(leftEyePin, OUTPUT);
    pinMode(rightEyePin, OUTPUT);
    
    // Start with both eyes off
    digitalWrite(leftEyePin, LOW);
    digitalWrite(rightEyePin, LOW);
}
void Eyes::startSequence() {
    isInSequence = true;
    blinkCount = 0;
    eyesCurrentlyOn = true;
    sequenceStartTime = millis();
    lastBlinkTime = sequenceStartTime;
    setBoth(false);  // Start with eyes off
}
void Eyes::update() {
    if (!isInSequence) return;
    unsigned long currentTime = millis();
    
    // LEDs on
    if (!eyesCurrentlyOn && currentTime - lastBlinkTime >= BLINK_INTERVAL) {
        setBoth(true);
        eyesCurrentlyOn = true;
        lastBlinkTime = currentTime;
        blinkCount++;
    }
    // LEDS off
    else if (eyesCurrentlyOn && currentTime - lastBlinkTime >= BLINK_DURATION) {
        setBoth(false);
        eyesCurrentlyOn = false;

    }
    // If we've completed all blinks
    if (blinkCount >= totalBlinks && !eyesCurrentlyOn) {
        isInSequence = false;  // End sequence
    }
}

void Eyes::blinkBoth() {
    digitalWrite(leftEyePin, LOW);
    digitalWrite(rightEyePin, LOW);
    leftEyeOn = rightEyeOn = false;
    delay(100);  // Brief blink
    digitalWrite(leftEyePin, HIGH);
    digitalWrite(rightEyePin, HIGH);
    leftEyeOn = rightEyeOn = true;
}

void Eyes::wink(bool leftEye) {
    if (leftEye) {
        digitalWrite(leftEyePin, LOW);
        leftEyeOn = false;
        delay(100);
        digitalWrite(leftEyePin, HIGH);
        leftEyeOn = true;
    } else {
        digitalWrite(rightEyePin, LOW);
        rightEyeOn = false;
        delay(100);
        digitalWrite(rightEyePin, HIGH);
        rightEyeOn = true;
    }
}

void Eyes::setBoth(bool on) {
    digitalWrite(leftEyePin, on ? HIGH : LOW);
    digitalWrite(rightEyePin, on ? HIGH : LOW);
    leftEyeOn = rightEyeOn = on;
}

void Eyes::setLeft(bool on) {
    digitalWrite(leftEyePin, on ? HIGH : LOW);
    leftEyeOn = on;
}

void Eyes::setRight(bool on) {
    digitalWrite(rightEyePin, on ? HIGH : LOW);
    rightEyeOn = on;
}

bool Eyes::isFinished(){
  return !isInSequence;
}
