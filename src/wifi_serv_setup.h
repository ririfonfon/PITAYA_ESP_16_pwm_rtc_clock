#ifndef wifi_serv_setup_h
#define wifi_serv_setup_h

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

#include <WebSocketsServer.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <SPIFFS.h>

// WiFi network settings

const char *host = "pitaya";
const char *ssid = "pitaya";
const char *password = "";

#define MAX_CLIENT 2

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

#include "fonction_spiffs.h"
#include "fonction_web_socket.h"
#include "srv_handle.h"

//////////////////////////////////////////init_wifi////////////////////////////////////////
void init_wifi()
{
    WiFi.softAPsetHostname(host);
    // IPAddress Ip(192, 168, 4, 1);
    // IPAddress NMask(255, 0, 0, 0);
    // WiFi.softAPConfig(Ip, Ip, NMask);
    WiFi.softAP(ssid, password);
    WiFi.begin();
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);

    //////////////////////////////////////////////////// SPIFFS
    SPIFFS.begin();
    listDir(SPIFFS, "/", 0);

    //////////////////////////////////////////////////// SERVER INIT
    //list directory
    server.on("/list", HTTP_GET, handleFileList);
    //load editor
    server.on("/edit", HTTP_GET, []()
              {
                  if (!handleFileRead("/edit.html"))
                      server.send(404, "text/plain", "FileNotFound");
              });
    //create file
    server.on("/edit", HTTP_PUT, handleFileCreate);
    //delete file
    server.on("/edit", HTTP_DELETE, handleFileDelete);
    //first callback is called after the request has ended with all parsed arguments
    //second callback handles file uploads at that location
    server.on(
        "/edit", HTTP_POST, []()
        { server.send(200, "text/plain", ""); },
        handleFileUpload);

    //called when the url is not defined here
    //use it to load content from SPIFFS
    server.onNotFound([]()
                      {
                          if (!handleFileRead(server.uri()))
                              server.send(404, "text/plain", "FileNotFound");
                      }); //server.onNotFound

    // start webSocket server
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
    webSocket.enableHeartbeat(15000, 3000, 2);

    if (MDNS.begin(host))
    {
#ifdef DEBUG
        Serial.println("MDNS responder started");
#endif
    }

    // handle index
#ifdef DEBUG
    Serial.println("HTTP server setup");
#endif

    //Serveur
    server.on("/set", srv_handle_set);
    server.serveStatic("/", SPIFFS, "/pitaya.html");
    server.serveStatic("/pitaya.html", SPIFFS, "/pitaya.html");
    server.serveStatic("/pitaya.js", SPIFFS, "/pitaya.js");
    server.serveStatic("/pitaya.css", SPIFFS, "/pitaya.css");
    server.serveStatic("/favicon.ico", SPIFFS, "/favicon.ico");
    server.begin();

    MDNS.addService("http", "tcp", 80);
    MDNS.addService("ws", "tcp", 81);

    //////////////////////////////////////////////////// EEPROM
    init_eeprom();
    eeprom_read();

#ifdef DEBUG
    Serial.println("HTTP server started.");
    Serial.println("ready!");
    Serial.println();
    Serial.print("Configuring access point...");
    Serial.println("AP IP address: ");
    Serial.println(myIP);
    Serial.println("HTTP server OK");
#endif

    init_w = true;

} //init_wifi
#endif