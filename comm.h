#ifndef KERAPOLIS_COMM_H
#define KERAPOLIS_COMM_H

#include <WiFi.h>
#include <WiFiUdp.h>

// WiFi network name and password:
static const char * networkName = "KERAPolis";    // fn0102453cs61
static const char * networkPswd = "9876543210";   // hacks20199161

//IP address to send UDP data to:
// either use the ip address of the server or 
// a network broadcast address
static const char * udpAddress = "10.1.14.130";
static const int udpPort = 54321;

// Client Functions

//Are we currently connected?
extern boolean connected;

//The udp library class
extern WiFiUDP udp;

void connectToWiFi(const char * ssid, const char * pwd);

//wifi event handler
void WiFiEvent(WiFiEvent_t event);

#endif
