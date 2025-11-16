/*TODO:

Zeitevents für jeden track, um zähne mit ton zu synchronisieren
Text und pixelart überlegen : https://github.com/Bodmer/TFT_eSPI/blob/master/examples/Generic/drawXBitmap/drawXBitmap.ino
wifi-interrupt hinzufügen

set doorbell flag on wifi-message received
müppi-miau
plektrum verlängern
Augen abwechselnd blinken
wenn dritter touch-sensor gedrückt:   spiele löwenzahn-melodie

*/

#include "Eyes.h"
#include "Teeth.h"
#include "Arm.h"
#include "Speech.h"
#include "TFT.h"
#include "Wifi_Module.h"
//#include "espnow.h"

// Pin definitions
const int DOORBELL_PIN = 0;  // Example interrupt-capable pin
const int TURN_LEFT_PIN = 32;
const int TURN_RIGHT_PIN = 33;
const int TOUCH_THRESHOLD = 20;


// Volatile variables for interrupt safety
volatile bool doorbellRung = false;
volatile bool turnLeft, turnRight;
volatile unsigned long lastInterruptTime = 0;

void clock_detach_interrupts();
void clock_attach_interrupts();

class KlausDieterDoorbell {
private:
    // Hardware interface objects (similar to previous example)
    Eyes eyes;
    Teeth teeth;
    Arm arm;
    Speech speech;
    Tft tft;
    Wifi_Module wifi;
    //MP3Player soundModule;

    // Debounce configuration
    const unsigned long DEBOUNCE_DELAY = 50;  // Milliseconds to prevent multiple triggers

    enum DoorbellState {
        IDLE,
        PERFORMING_SEQUENCE,
        TURNING_ARM_LEFT,
        TURNING_ARM_RIGHT,
        
    };
    DoorbellState currentState = IDLE;

public:
     KlausDieterDoorbell() 
        : eyes(),  // Properly initialize Eyes with pins
          teeth(),
          arm(),
          speech(),
          tft(),
          wifi()
    {}

    void initialize() {
        // Setup interrupt pin with internal pull-up
        pinMode(DOORBELL_PIN, INPUT);
        
        // Attach interrupt - FALLING means triggered when pin goes from HIGH to LOW
        attachInterrupt(digitalPinToInterrupt(DOORBELL_PIN), doorbellInterruptHandler, FALLING);

        //attach espnow interrupt
        

        touchAttachInterrupt(TURN_LEFT_PIN, turnLeftInterruptHandler, TOUCH_THRESHOLD);
        touchAttachInterrupt(TURN_RIGHT_PIN, turnRightInterruptHandler, TOUCH_THRESHOLD);

        //TODO: in new ISR, enter state that moves left until the capacitance is above threshold again

        // Initialize other hardware components
        eyes.setup();
        teeth.setup();
        arm.setup();
        speech.setup();
        tft.setup();
        wifi.setup();
    }

    // Static interrupt handler to set flag
    static void doorbellInterruptHandler() {
        // Basic debounce logic
        unsigned long currentTime = millis();
        if (currentTime - lastInterruptTime > 250) {
            doorbellRung = true;
            lastInterruptTime = currentTime;
        }
    }


    static void turnLeftInterruptHandler() {
        // Basic debounce logic
        unsigned long currentTime = millis();
        if (currentTime - lastInterruptTime > 250) {
            turnLeft = true;
            lastInterruptTime = currentTime;
        }
    }

    static void turnRightInterruptHandler() {
        // Basic debounce logic
        unsigned long currentTime = millis();
        if (currentTime - lastInterruptTime > 250) {
            turnRight = true;
            lastInterruptTime = currentTime;
        }
    }

    void update() {
        wifi.update();


        // Check if doorbell was rung (set by interrupt)
        if (doorbellRung && currentState == IDLE) {
            performDoorbellSequence();
        }

        // Manage concurrent actions if in performance state
        if (currentState == PERFORMING_SEQUENCE) {
            runConcurrentActions();
        }
        if (turnLeft && currentState == IDLE){
            StartTurnArmLeft();
            
        }
        if (turnRight && currentState == IDLE){
            StartTurnArmRight();
        }
        if (currentState == TURNING_ARM_LEFT){
          if(touchRead(TURN_LEFT_PIN)<TOUCH_THRESHOLD){
            arm.moveLeft(100);
          } else {
            currentState = IDLE;
          }
        }
        if (currentState == TURNING_ARM_RIGHT){
          if(touchRead(TURN_RIGHT_PIN)<TOUCH_THRESHOLD){
            arm.moveRight(100);
          } else {
            currentState = IDLE;
          }
        }
    }
    void StartTurnArmLeft(){
      currentState=TURNING_ARM_LEFT;
      turnLeft = false;   
      }
    void StartTurnArmRight(){
      currentState=TURNING_ARM_RIGHT;
      turnRight = false;   
      }
      
    void performDoorbellSequence() {
        // Disable further interrupts during sequence
        detachInterrupt(digitalPinToInterrupt(DOORBELL_PIN));
        touchDetachInterrupt(TURN_LEFT_PIN);
        touchDetachInterrupt(TURN_RIGHT_PIN);
        clock_detach_interrupts();

        // Trigger all doorbell actions
        arm.startSequence();
        eyes.startSequence();
        teeth.startSequence();
        speech.startSequence();
        tft.startSequence();
        //Serial.println("starting sequence.");
        // Change state
        currentState = PERFORMING_SEQUENCE;
        doorbellRung = false;  // Reset interrupt flag
    }

    void runConcurrentActions() {
        arm.update();
        eyes.update();
        teeth.update();
        speech.update();
        tft.update();

        // Check if sequence is complete
        if(eyes.isFinished()&&teeth.isFinished()&&arm.isFinished()&&speech.isFinished()&&tft.isFinished()){
            currentState = IDLE;
            Serial.println("Reenabling interrupts");
            // Re-enable interrupt after sequence completes
            attachInterrupt(digitalPinToInterrupt(DOORBELL_PIN), doorbellInterruptHandler, FALLING);
            touchAttachInterrupt(TURN_LEFT_PIN, turnLeftInterruptHandler, TOUCH_THRESHOLD);
            touchAttachInterrupt(TURN_RIGHT_PIN, turnRightInterruptHandler, TOUCH_THRESHOLD);
            clock_attach_interrupts();
            
            //TODO: also de- and reattach the wifi-interrupt
        }
    }
    void update_time_display(){
      tft.update_time();
    }
};

// Global instance
KlausDieterDoorbell doorbell;


void setup() {
  Serial.begin(115200);
  doorbell.initialize();
  clock_setup();

  
  
}

void loop() {
    doorbell.update();
    clock_loop();
}
