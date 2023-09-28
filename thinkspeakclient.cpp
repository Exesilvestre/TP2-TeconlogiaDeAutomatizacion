#include "thinkspeakclient.h"
#include<UniversalTelegramBot.h>
#include<HTTPClient.h>
#include<Arduino.h>
#include <WiFi.h>

ThinkSpeakClient::ThinkSpeakClient(long channel, char* apikey){
  _channel = channel;
  _apikey = strdup(apikey); // Copia el API key en una nueva cadena
  _baseurl = "https://api.thingspeak.com/update?api_key=";
  _url = _baseurl + _apikey;
}

String ThinkSpeakClient::sendToDashboard(){
  if(WiFi.status() == WL_CONNECTED){
      delay(1000);

      int rand = random(1,100);
      String end_point = _url + "&field1=" +  String(rand);
      HTTPClient client;

      client.begin(end_point.c_str());
      int codigo_estado = client.GET();

      String msj = "";

      if(codigo_estado == 200){
        Serial.println("HTTP Status code: ");
        Serial.println(codigo_estado);
        Serial.println(client.getString());
        msj = "Numero generado con exito";
      }else{
        Serial.println("HTTP Error Status code: ");
        msj = "Error " + String(codigo_estado);
      }
      client.end();

      return msj;
    }
}