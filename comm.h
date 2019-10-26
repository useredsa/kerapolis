#ifndef KERAPOLIS_COMM_H
#define KERAPOLIS_COMM_H

#include <WiFi.h>
#include <WiFiUdp.h>
#include <AsyncUDP.h>

// WiFi IDs
//static const char *networkName = "UJI";
//static const char *networkPswd = "";
// fn0102453cs61
// hacks20199161
static const char * networkName = "KERAPolis";
static const char * networkPswd = "9876543210";
static const char *serverAddress = "192.168.0.108";
static const int udpPort = 54321;

// Client Functions

//Are we currently connected?
extern boolean connected;

//The udp library class
extern WiFiUDP udp;
extern AsyncUDP ethUDP;

void connectToWiFi(const char * ssid, const char * pwd);

//wifi event handler
void WiFiEvent(WiFiEvent_t event);

/**
 * @brief Sends KERAPolisStatus message to the server. Use when connected.
 */
void sendCityStatus();

#endif
