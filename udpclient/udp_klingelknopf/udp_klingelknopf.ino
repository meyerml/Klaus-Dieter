#include <WiFi.h>
#include <WiFiUdp.h>


#define LED1PIN 12
#define LED2PIN 14
/* WiFi network name and password */

const char* ssid = "WG 2.2";
const char* pwd = "opWF6nvt6#v";


// IP address to send UDP data to.
// it can be ip address of the server or 
// a network broadcast address
// here is broadcast address
const char * udpAddress = "10.202.2.215";
const char * broadcastAddress = "10.202.2.255";
const int udpPort = 1234;

// Define the button pin
const int buttonPin = 27;  // Use the appropriate pin for your setup

// Variables for debouncing
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
int lastButtonState = HIGH;
int buttonState;
//create UDP instance
WiFiUDP udp;

void setup(){
  pinMode(LED1PIN, OUTPUT);
  pinMode(LED2PIN, OUTPUT);
  Serial.begin(115200);
     pinMode(buttonPin, INPUT_PULLUP);  // Use INPUT_PULLUP to avoid floating input

  
  //Connect to the WiFi network
   WiFi.begin(ssid, pwd);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop(){
  




    int reading = digitalRead(buttonPin);

  // Check to see if you just pressed the button
  // (i.e., the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      // Only send the message if the new button state is LOW (pressed)
      if (buttonState == LOW) {

        // Send message via UDP
        //data will be sent to server

          digitalWrite(LED1PIN, HIGH);
          digitalWrite(LED2PIN, HIGH);

          Serial.println("sending udp packet");
          uint8_t buffer[50] = "RING";
          //This initializes udp and transfer buffer
          udp.beginPacket(udpAddress, udpPort);
          udp.write(buffer, 4);
          udp.endPacket();
          memset(buffer, 0, 50);
          //processing incoming packet, must be called before reading the buffer
          udp.parsePacket();
          //receive response from server, it will be HELLO WORLD
          if(udp.read(buffer, 50) > 0){
            Serial.print("Server to client: ");
            Serial.println((char *)buffer);
          }
          delay(1000);
          digitalWrite(LED1PIN, LOW);
          digitalWrite(LED2PIN, LOW);
      }
    }
  }

  lastButtonState = reading;
}
