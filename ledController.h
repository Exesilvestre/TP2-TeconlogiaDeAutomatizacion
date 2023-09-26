#ifndef LEDCONTROLLER
#define LEDCONTROLLER

#include <Arduino.h>

class LedController {
  private: 
    int _ledPin;
  public: 
    LedController(int pinLed);
    void turnOn();
    void turnOff();
};

#endif