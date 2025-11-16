#include "Wifi_Module.h"


const char *ssid = "WG 2.2";
const char *password = "opWF6nvt6#v";
Wifi_Module::Wifi_Module(){
  //nix
}

AsyncUDP udp;
extern bool doorbellRung;


void Wifi_Module::setup() {
  Serial.println("Booting");
  //WiFi.mode(WIFI_STA);
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection to Wifi Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }


  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH) {
        type = "sketch";
      } else {  // U_SPIFFS
        type = "filesystem";
      }

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) {
        Serial.println("Auth Failed");
      } else if (error == OTA_BEGIN_ERROR) {
        Serial.println("Begin Failed");
      } else if (error == OTA_CONNECT_ERROR) {
        Serial.println("Connect Failed");
      } else if (error == OTA_RECEIVE_ERROR) {
        Serial.println("Receive Failed");
      } else if (error == OTA_END_ERROR) {
        Serial.println("End Failed");
      }
    });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());


  //udp server setup:
  if (udp.listen(1234)) {
  Serial.print("WiFi connected. UDP Listening on IP: ");
  Serial.println(WiFi.localIP());
  udp.onPacket([](AsyncUDPPacket packet) {
  Serial.print("UDP Packet Type: ");
  Serial.print(packet.isBroadcast() ? "Broadcast" : packet.isMulticast() ? "Multicast" : "Unicast");
  Serial.print(", From: ");
  Serial.print(packet.remoteIP());
  Serial.print(":");
  Serial.print(packet.remotePort());
  Serial.print(", To: ");
  Serial.print(packet.localIP());
  Serial.print(":");
  Serial.print(packet.localPort());
  Serial.print(", Length: ");
  Serial.print(packet.length()); //dlzka packetu
  Serial.print(", Data: ");
  Serial.write(packet.data(), packet.length());
  Serial.println();
  String myString = (const char*)packet.data();
  if (myString.startsWith("RING")) {
  Serial.println("Doorbell Rung");
  doorbellRung = true;
  }
  packet.printf("Got %u bytes of data", packet.length());
});
}


}  

void Wifi_Module::update(){
  ArduinoOTA.handle();
}
