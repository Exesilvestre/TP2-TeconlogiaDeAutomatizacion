#include "display.h" 


  Display::Display(int w, int h, int reset): _display(w, h, &Wire){
    //Inicializar todos los atributos de display
  }

  void Display::init(int direccion){
    _display.begin(direccion, true);
    _display.setTextSize(1);
    _display.setTextColor(SH110X_WHITE);
  }

  void Display::showOutput(String text){
    _display.clearDisplay();
    _display.setCursor(0, 0);
    _display.println(text);
    _display.display(); //refresh
  }

  void Display::updateDisplayDHTData(float t, float h){
  _display.clearDisplay();
  _display.setCursor(0,0);
  _display.printf("Temp: %.2f\nHum: %.2f", t, h);
  _display.printf("\n\nSeleccione una opcion: \n"
                    "1. Tomar valores\n"
                    "2. Volver atras");
  _display.display();
  }

  void Display::updateDisplayProte(float valor, float voltaje){
  _display.clearDisplay();
  _display.setCursor(0,0);
  _display.printf("Valor: %.2f\nVoltaje: %.2f", valor, voltaje);
  _display.printf("\n\nSeleccione una opcion: \n"
                    "1. Tomar valores\n"
                    "2. Volver atras");
  _display.display();
  }


  void Display::clearDisplay(){
    _display.clearDisplay();
    _display.display();
  }
