#include "Arm.h"

const int LEFT_PIN = 17;
const int RIGHT_PIN = 16;
const unsigned long MOVE_DURATION_LEFT = 2000;  
const unsigned long MOVE_DURATION_RIGHT = 2200;  

const unsigned long INCREMENT = 100;

Arm::Arm()
    : leftPin(LEFT_PIN), rightPin(RIGHT_PIN), moveDurationLeft(MOVE_DURATION_LEFT), moveDurationRight(MOVE_DURATION_RIGHT), increment(INCREMENT),
      isMoving(false), isInSequence(false) {}

void Arm::setup() {
  pinMode(leftPin,OUTPUT);
  pinMode(rightPin,OUTPUT);
  digitalWrite(leftPin,LOW);
  digitalWrite(rightPin,LOW);
}

void Arm::startSequence() {
    isInSequence = true;
    sequenceStartTime = millis();
}

void Arm::update() {
    //Serial.println("1");
    if (!isInSequence) return;

    unsigned long currentTime = millis();
    unsigned long elapsedTime = currentTime - sequenceStartTime;

    if (elapsedTime >= (moveDurationLeft + moveDurationRight)){
      digitalWrite(leftPin, LOW);
      digitalWrite(rightPin, LOW);
      isInSequence = false;
    } else if (elapsedTime >= moveDurationRight) {
        moveLeft(increment);   // End sequence
    
    } else if (elapsedTime < moveDurationRight) {
        moveRight(increment);  // move arm right in a small increment
    }    


}

bool Arm::isFinished() {
    return !isInSequence;
}

void Arm::moveLeft(unsigned long increment) {
    if (isMoving) return;
    isMoving = true;
    moveStartTime = millis();
    digitalWrite(rightPin, LOW);
    digitalWrite(leftPin, HIGH);
    delay(increment);  // Move in increments
    digitalWrite(leftPin, LOW);
    //Serial.println("1");
    isMoving = false;
}


void Arm::moveRight(unsigned long increment) {
    if (isMoving) return;
    isMoving = true;
    moveStartTime = millis();
    digitalWrite(leftPin, LOW);
    digitalWrite(rightPin, HIGH);
    delay(increment);  // Move in increments
    digitalWrite(rightPin, LOW);
    isMoving = false;
}
