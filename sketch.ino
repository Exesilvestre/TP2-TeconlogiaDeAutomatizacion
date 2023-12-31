#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <DHT.h>
#include "ledController.h"
#include "display.h"
#include "poteController.h"
#include "thinkspeakclient.h"

void mensajesNuevos(int numerosMensajes);
String reemplazarGuiones(String txt);

#define LED1 23
#define LED2 2
#define PIN_SENSOR 14
#define POTE 32

// Para el punto opcional
const long CHANNEL = 2285543;
char* API_KEY ="I05Z3U0RL0AGK1SJ";

//URL base: 
const String BASEURL = "https://api.thingspeak.com/update?api_key="; 

String url = BASEURL + API_KEY;


LedController ledController(LED1);
Display myDisplay(128, 64, -1);
DHT sensor(PIN_SENSOR, DHT22);
PoteController pote(POTE);
ThinkSpeakClient tsclient(CHANNEL, API_KEY);

//Necesitamos ssid pass token de nuestro bot

const char* SSID = "Wokwi-GUEST";
const char* PASS = "";
const char* BOT_TOKEN  = "6368598380:AAE2Av-hRD-5m8jlj3GLtIa7nXfK3r36quQ";
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

  delay(1000); // this speeds up the simulation
}




void mensajesNuevos(int numerosMensajes){

  Serial.println("Mensaje!");


  //Por cada mensaje pido el id de la comunicación, el texto y ver que me mandó
  for(int i = 0; i< numerosMensajes; i++)
  {
    String menu_start = "Bienvenido al Trabajo Practico N2 \n\n"
                               "/led<led><on/off> - permite encender o apagar un led de la placa. "
                               "Indique en led el GPIO correspondiente al led verde (23) o al azul (2) y la acción on/off.\n\n"
                               "/dht22 - permite informar los valores de humedad y temperatura del sensor \n\n"
                               "/pote - permite informar el valor de voltaje (0-3.3v) segun la lectura del potenciometro \n\n"
                               "/display<mensaje> - permite mostrar el contenido de un mensaje originado desde el bot"
                               " en el display de la placa de desarrollo.\n\n"
                               "/platiot - envia un valor aleatorio a la API entre 1 y 100";

    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;
    Serial.println(text);

    //Procesar comandos
    if(text == "/led23on")
    {
      digitalWrite(LED1, HIGH);
      bot.sendMessage(chat_id, "LED verde encendido!", "");
    }
    
    else if(text == "/led23off")
    {
      digitalWrite(LED1, LOW);
      bot.sendMessage(chat_id, "LED verde apagado!", "");
    }

    else if(text == "/led2on")
    {
      digitalWrite(LED2, HIGH);
      bot.sendMessage(chat_id, "LED azul encendido!", "");
    }
    
    else if(text == "/led2off")
    {
      digitalWrite(LED2, LOW);
      bot.sendMessage(chat_id, "LED azul apagado!", "");
    }

    else if(text == "/dht22"){
      float temp = sensor.readTemperature();
      float hum = sensor.readHumidity();

      bot.sendMessage(chat_id, "Temp: " + String(temp) + "ºc - " + "Hum: " + String(hum) + "%", "");
    }

    else if(text == "/pote"){
      float voltaje = pote.obtenerVoltaje();
      bot.sendMessage(chat_id, "Valor de voltaje de potenciometro: " + String(voltaje) + "v");
    }

    else if(text == "/start"){
      bot.sendMessage(chat_id, menu_start, "");
    }

    else if(text.length() >= 8){
      if(text.substring(0, 8) == "/display"){
        String msj = text.substring(8, text.length());
        String msj_formateado = reemplazarGuiones(msj);
        myDisplay.showOutput(msj_formateado);
        bot.sendMessage(chat_id, "Ver mensaje en el display.");
      }
    }

    else if(text == "/platiot"){
      String msj = tsclient.sendToDashboard();
      Serial.println(msj);
    }
    
    else{
      bot.sendMessage(chat_id, "El comando no es valido.");
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

