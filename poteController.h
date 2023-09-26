#ifndef _POTECONTROLLER
#define _POTECONTROLLER


#include<Arduino.h>


class PoteController{
  private:
    int _potePin;
  public:
    PoteController(int pin);
    int obtenerValor();
    float obtenerVoltaje();
};




#endif
