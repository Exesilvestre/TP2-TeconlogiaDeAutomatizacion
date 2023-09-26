#include "ledController.h"
#include <Arduino.h>

LedController::LedController(int pinLed) : _ledPin(pinLed){
  pinMode(_ledPin, OUTPUT);
  digitalWrite(_ledPin, LOW);
}

void LedController::turnOn(){
  digitalWrite(_ledPin, HIGH);
}

void LedController::turnOff(){
  digitalWrite(_ledPin, LOW);
}