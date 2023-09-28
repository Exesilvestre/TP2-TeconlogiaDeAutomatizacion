#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <DHT.h>
#include "ledController.h"
#include "display.h"
#include "poteController.h"

void mensajesNuevos(int numerosMensajes);
String reemplazarGuiones(String txt);

#define LED1 23
#define LED2 2
#define PIN_SENSOR 14
#define POTE 32

// Para el punto opcional
const long CHANNEL = 2285543;
const char* API_KEY ="I05Z3U0RL0AGK1SJ";

//URL base: 
const String BASEURL = "https://api.thingspeak.com/update?api_key="; 

String url = BASEURL + API_KEY;


LedController ledController(LED1);
Display myDisplay(128, 64, -1);
DHT sensor(PIN_SENSOR, DHT22);
PoteController pote(POTE);

//Necesitamos ssid pass token de nuestro bot

const char* SSID = "Wokwi-GUEST";
const char* PASS = "";
const char* BOT_TOKEN  = "6394488729:AAF88olXJ9pCO5os9MBKdRSZl59YisIVn7s";
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
  
  /*
    PARA PUNTO OPCIONAL
    if(wifi.status() != WL_CONNECTED){
      delay(1000);

      int rand = random(1,100);
      String end_point = URL + "&field1="
      HTTPClient client;

      client.begin(end_point.c_str());
      int codigo_estado = client.GET();

      if(codigo_estado == 200){
        Serial.println("HTTP Status code: ");
        Serial.print(codigo_estado);
        Serial.println(client.getString());
      }else{
        Serial.println("HTTP Error Status code: ");
      }
      client.end();
    }
  */             

  delay(1000); // this speeds up the simulation
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
    if(text == "/led23on")
    {
      digitalWrite(LED1, HIGH);
      bot.sendMessage(chat_id, "LED verde encendido!", "");
    }
    
    if(text == "/led23off")
    {
      digitalWrite(LED1, LOW);
      bot.sendMessage(chat_id, "LED verde apagado!", "");
    }

    if(text == "/led2on")
    {
      digitalWrite(LED2, HIGH);
      bot.sendMessage(chat_id, "LED azul encendido!", "");
    }
    
    if(text == "/led2off")
    {
      digitalWrite(LED2, LOW);
      bot.sendMessage(chat_id, "LED azul apagado!", "");
    }

    if(text == "/dht22"){
      float temp = sensor.readTemperature();
      float hum = sensor.readHumidity();

      bot.sendMessage(chat_id, "Temp: " + String(temp) + "ºc - " + "Hum: " + String(hum) + "%", "");
    }

    if(text == "/pote"){
      float voltaje = pote.obtenerVoltaje();
      bot.sendMessage(chat_id, "Valor de voltaje de potenciometro: " + String(voltaje) + "v");
    }

    if(text.length() >= 8){
      if(text.substring(0, 8) == "/display"){
        String msj = text.substring(8, text.length());
        String msj_formateado = reemplazarGuiones(msj);
        myDisplay.showOutput(msj_formateado);
      }
    }

    if(text == "/platiot"){
      if(WiFi.status() == WL_CONNECTED){
      delay(1000);

      int rand = random(1,100);
      String end_point = url + "&field1=" +  String(rand);
      HTTPClient client;

      client.begin(end_point.c_str());
      int codigo_estado = client.GET();

      if(codigo_estado == 200){
        Serial.println("HTTP Status code: ");
        Serial.print(codigo_estado);
        Serial.println(client.getString());
      }else{
        Serial.println("HTTP Error Status code: ");
      }
      client.end();
    }
    }


    text = "";
  }
}

String reemplazarGuiones(String txt){
  for (size_t i = 0; i < txt.length(); ++i) {
            if (txt[i] == '_') {
                txt[i] = ' ';
            }
  }
  return txt;
}