#include <WiFi.h>
//#include <HttpClient.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <DHT.h>
#include "ledController.h"
#include "display.h"
#include "poteController.h"

void mensajesNuevos(int numerosMensajes);

#define LED1 23
#define LED2 2
#define PIN_SENSOR 14
#define POTE 12

LedController ledController(LED1);
Display myDisplay(128, 64, -1);
DHT sensor(PIN_SENSOR, DHT22);
PoteController pote(POTE);

//Necesitamos ssid pass token de nuestro bot

const char* SSID = "Silvestre";
const char* PASS = "Emilio08";
const char* BOT_TOKEN  = "5863319388:AAGEF5-8pPnmn5Lq-5L3_T6Ye9ZtNwslTNM";
const unsigned long tiempo = 500; //tiempo medio entre mensajes de escaneo

//Token de Telegram BOT se obtenienen desde Botfather en telegram
WiFiClientSecure secured_client; //Objeto clientSecure para un cliente seguro
UniversalTelegramBot bot(BOT_TOKEN, secured_client); //Inicializamos el bot, recibe la clave y el cliente seguro como parámetros
unsigned long tiempoAnterior; //última vez que se realizó el análisis de mensajes, para poder comparar con tiempo actual

void setup() {
  Serial.begin(115200);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
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
  //Si entre el momento actual y el tiempo anterior en el que hice el análisis se superó un segundo...
  //Pedimos el último mensaje recibido (su ID+1)
  if(millis() - tiempoAnterior > tiempo)
  {
    int numerosMensajes = bot.getUpdates(bot.last_message_received + 1);


    //Mientras que el numero de mensajes no sea 0
    //Llamamos a la función auxiliar con ese numero de mensaje para procesar los mensajes
    //Luego pregunto por novedad de mensaje nuevo por si llegó otro mientras procesábamos el anterior
    while(numerosMensajes)
    {
      Serial.println("Comando recibido");
      mensajesNuevos(numerosMensajes);
      numerosMensajes = bot.getUpdates(bot.last_message_received + 1);
    }

    tiempoAnterior = millis();
  }
  



  delay(10); // this speeds up the simulation
}




void mensajesNuevos(int numerosMensajes){

  Serial.println("Mensaje!");


  //Por cada mensaje pido el id de la comunicación, el texto y ver que me mandó
  for(int i = 0; i< numerosMensajes; i++)
  {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;
    Serial.println(text);

    //Procesar comandos
    if(text == "/ledoff")
    {
      digitalWrite(LED1, HIGH);
      bot.sendMessage(chat_id, "LED encendido!", "");
    }
    
    if(text == "/ledon")
    {
      digitalWrite(LED1, LOW);
      bot.sendMessage(chat_id, "LED apagado!", "");
    }
  }
}