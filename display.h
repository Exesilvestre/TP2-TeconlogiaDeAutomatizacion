#ifndef _DISPLAY
#define _DISPLAY

#include<Adafruit_SH110X.h>
#include<Wire.h>
#include<Arduino.h>

class Display { //clase wrapper de la pantalla
  private:
    Adafruit_SH1106G _display;
  public:
    Display(int w, int h, int reset);
    void init(int direccion);
    void showOutput(String text); 
    void updateDisplayDHTData(float t, float h);
    void updateDisplayProte(float valor, float voltaje);
    void clearDisplay();
};

#endif