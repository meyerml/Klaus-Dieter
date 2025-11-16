#ifndef WIFI_MODULE_H
#define WIFI_MODULE_H


#include <WiFi.h>
#include <ESPmDNS.h>
#include <NetworkUdp.h>
#include <ArduinoOTA.h>
#include "AsyncUDP.h"



class Wifi_Module{

  public:
    Wifi_Module();
    void setup();
    void update();

};
#endif
