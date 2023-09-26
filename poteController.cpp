#include "poteController.h"
#include <Arduino.h>

PoteController::PoteController(int pin):_potePin(pin){
  pinMode(_potePin, INPUT);
}

int PoteController::obtenerValor(){
  return analogRead(_potePin)/16; 
}

float PoteController::obtenerVoltaje(){
  return (obtenerValor() * 3.3) / 255;
}