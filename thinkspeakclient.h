#ifndef _THINKSPEAKCLIENT
#define _THINKSPEAKCLIENT

#include<UniversalTelegramBot.h>
#include<HTTPClient.h>
#include<Arduino.h>
#include <WiFi.h>

class ThinkSpeakClient{
  private:
    long _channel;
    char* _apikey;
    String _baseurl;
    String _url;
  public:
    ThinkSpeakClient(long channel, char* apikey);
    String sendToDashboard();
};

#endif