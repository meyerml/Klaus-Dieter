#include "Teeth.h"

const int SERVO_PIN = 4;
const int CLOSED_POSITION_1 = 155;
const int CLOSED_POSITION_2 = 65;
const int OPEN_POSITION = (CLOSED_POSITION_1+CLOSED_POSITION_2)/2;
const unsigned long MOVE_DURATION = 2000;  // Duration for opening/closing teeth


Teeth::Teeth()
    : servoPin(SERVO_PIN), closedPosition1(CLOSED_POSITION_1), closedPosition2(CLOSED_POSITION_2), openPosition(OPEN_POSITION), moveDuration(MOVE_DURATION),
      isMoving(false), isInSequence(false), sequenceStep(0) {}

void Teeth::setup() {
    servo.attach(servoPin, 500, 2500);
    servo.write(closedPosition1);  // Start with teeth closed
}

void Teeth::startSequence() {
    //Serial.println("start");
    isInSequence = true;
    sequenceStep = 0;
    sequenceStartTime = millis();
    moveToPosition(closedPosition1);  
}

void Teeth::update() {
    if (!isInSequence) return;

    unsigned long currentTime = millis();
    unsigned long elapsedTime = currentTime - sequenceStartTime;
    if (elapsedTime >= moveDuration) {
    
        isInSequence = false;  // End sequence
        Serial.println("Done");
    } else if (elapsedTime >= moveDuration * 0.5) {

        moveToPosition(closedPosition1);  // Close teeth halfway through the sequence
        //Serial.println("closing");
    } else{
        moveToPosition(openPosition);  // Close teeth halfway through the sequence
        //Serial.println("opening");

        //sequenceStep = 1;
    //}else if (sequenceStep == 1 && elapsedTime >= moveDuration *2.0/3.0) {
    }
    //if (sequenceStep == 0 && elapsedTime >= moveDuration / 3.0) {
    //if (elapsedTime >= moveDuration / 3.0) {
      //  moveToPosition(openPosition);  // Close teeth halfway through the sequence
        //sequenceStep = 1;
    //}else if (sequenceStep == 1 && elapsedTime >= moveDuration *2.0/3.0) {
    //}else if (elapsedTime >= moveDuration *2.0/3.0) {

      //  moveToPosition(closedPosition1);  // Close teeth halfway through the sequence
        //sequenceStep = 2;
    //} else if (sequenceStep == 2 && elapsedTime >= moveDuration) {
    //} else if (elapsedTime >= moveDuration) {

      //  isInSequence = false;  // End sequence


    //}
}

bool Teeth::isFinished() {
    return !isInSequence;
}

void Teeth::moveToPosition(int position) {


    if (isMoving) return;
    isMoving = true;
    moveStartTime = millis();
    int startPosition = servo.read();
    int increment = (position - startPosition) / 8;  // Move in increments
    //Serial.println(increment);
    //Serial.println(position);
    //Serial.println(startPosition);
    if (increment != 0){
    for (int i = startPosition; (increment > 0) ? (i <= position) : (i >= position); i += increment) {
        servo.write(i);
        //Serial.println(i);
        delay(10);  // Small delay for smooth movement
    }
    }
    //servo.write(position);  // Ensure final position is set
    isMoving = false;
}
