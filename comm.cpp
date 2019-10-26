#include "comm.h"
#include "JSONParser.h"
#include "ETH.h"
#include <string>

boolean connected = false;
WiFiUDP udp;
AsyncUDP ethUDP;

void connectToWiFi(const char * ssid, const char * pwd){
  Serial.println("Connecting to WiFi network: " + String(ssid));
  // delete old config
  WiFi.disconnect(true);
  //register event handler
  WiFi.onEvent(WiFiEvent);
  
  //Initiate connection
  WiFi.begin(ssid, pwd);

  Serial.println("Waiting for WIFI connection...");

  ETH.begin();
  ETH.config(IPAddress(192,168,0,42), IPAddress(192,168,0,1), IPAddress(255,255,255,0));
}

//wifi event handler
void WiFiEvent(WiFiEvent_t event){
    switch(event) {
      case SYSTEM_EVENT_STA_GOT_IP:
          //When connected set 
          Serial.print("WiFi connected! IP address: ");
          Serial.println(WiFi.localIP());  
          //initializes the UDP state
          //This initializes the transfer buffer
          udp.begin(WiFi.localIP(), udpPort);
          connected = true;
          break;
      case SYSTEM_EVENT_STA_DISCONNECTED:
          Serial.println("WiFi lost connection");
          connected = false;
          break;
      default: break;
    }
}

void sendCityStatus() {
  udp.beginPacket("192.168.0.255", udpPort);
  std::string s = encodeStatus(); //TODO better use string reference (string&) or char*
  udp.printf("%s", s.c_str());
  udp.endPacket();
  Serial.println(s.c_str());
}
