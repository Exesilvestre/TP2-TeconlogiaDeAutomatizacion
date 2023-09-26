#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <DHT.h>
#include "ledController.h"
#include "display.h"
#include "poteController.h"



#define LED 23
#define LED 2
#define PIN_SENSOR 14
#define POTE 12

LedController ledController(LED);
Display myDisplay(128, 64, -1);
DHT sensor(PIN_SENSOR, DHT22);
PoteController pote(POTE);

const char* SSID = "Silvestre";
const char* PASS = "Emilio08";
const char* BOT_TOKEN  = "6368598380:AAE2Av-hRD-5m8jlj3GLtIa7nXfK3r36quQ";
const unsigned long tiempo = 500; //tiempo medio entre mensajes de escaneo

//Token de Telegram BOT se obtenienen desde Botfather en telegram
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long tiempoAnterior; //última vez que se realizó el análisis de mensajes

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(LED, OUTPUT);
  pinMode(POTE, INPUT);
  
  sensor.begin();

  myDisplay.init(0x3C);

  Serial.print("Conectando a la red ");
  WiFi.begin(SSID, PASS);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); //Agregar certificado raíz para api.telegram.org
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nConectado a la red wifi. Dirección IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10); // this speeds up the simulation
}
