#ifndef mqtt_tool
#define mqtt_tool

#include <WiFi.h>
extern "C"
{
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
}
#include <AsyncMqttClient.h>

#define WIFI_SSID "riri_new"
#define WIFI_PASSWORD "B2az41opbn6397"

#define MQTT_HOST IPAddress(192, 168, 0, 22)
#define MQTT_PORT 1883

AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;

char MQTT_TOPIC[] = "******************************";
String mqtt_topic;
String MQTT = "gps/";
String MQTT_SAT = "/sat";
String MQTT_ALT = "/alt";
String MQTT_LONG = "/long";
String MQTT_LAT = "/lat";
String MQTT_TIME_ON = "/timeon";
String MQTT_TIME_OFF = "/timeoff";
boolean wifihasFix = false;

void connectToWifi()
{
    Serial.println("Connecting to Wi-Fi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (!wifihasFix)
    {
        Serial.print(".");
        delay(1000);
    }
}

void connectToMqtt()
{
    Serial.println("Connecting to MQTT...");
    mqttClient.connect();
}

void WiFiEvent(WiFiEvent_t event)
{
    Serial.printf("[WiFi-event] event: %d\n", event);
    switch (event)
    {
    case SYSTEM_EVENT_STA_GOT_IP:
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        wifihasFix = true;
        connectToMqtt();
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        Serial.println("WiFi lost connection");
        xTimerStop(mqttReconnectTimer, 0); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
        xTimerStart(wifiReconnectTimer, 0);
        break;
    }
}

void onMqttConnect(bool sessionPresent)
{
    Serial.println("Connected to MQTT.");
    Serial.print("Session present: ");
    Serial.println(sessionPresent);
    mqtt_topic = String(MQTT) + String(ID);
    mqtt_topic.toCharArray(MQTT_TOPIC, mqtt_topic.length() + 1);
    uint16_t packetIdSub = mqttClient.subscribe(MQTT_TOPIC, 2);
    Serial.print("Subscribing at QoS 2, packetId: ");
    Serial.print(packetIdSub);
    Serial.print(" TOPIC : ");
    Serial.println(MQTT_TOPIC);

    mqtt_topic = String(MQTT) + String("all");
    mqtt_topic.toCharArray(MQTT_TOPIC, mqtt_topic.length() + 1);
    uint16_t packetIdSub2 = mqttClient.subscribe(MQTT_TOPIC, 2);
    Serial.print("Subscribing at QoS 2, packetId: ");
    Serial.print(packetIdSub2);
    Serial.print(" TOPIC : ");
    Serial.println(MQTT_TOPIC);

    // mqttClient.publish(MQTT_TOPIC, 0, true, "test 1");
    //   Serial.println("Publishing at QoS 0");
    //   uint16_t packetIdPub1 = mqttClient.publish("test/lol", 1, true, "test 2");
    //   Serial.print("Publishing at QoS 1, packetId: ");
    //   Serial.println(packetIdPub1);
    //   uint16_t packetIdPub2 = mqttClient.publish("test/lol", 2, true, "test 3");
    //   Serial.print("Publishing at QoS 2, packetId: ");
    //   Serial.println(packetIdPub2);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
{
    Serial.println("Disconnected from MQTT.");

    if (WiFi.isConnected())
    {
        xTimerStart(mqttReconnectTimer, 0);
    }
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos)
{
    Serial.println("Subscribe acknowledged.");
    Serial.print("  packetId: ");
    Serial.println(packetId);
    Serial.print("  qos: ");
    Serial.println(qos);
}

void onMqttUnsubscribe(uint16_t packetId)
{
    Serial.println("Unsubscribe acknowledged.");
    Serial.print("  packetId: ");
    Serial.println(packetId);
}

void onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
    Serial.println("Publish received.");
    Serial.print("  topic: ");
    Serial.println(topic);
    Serial.print("  qos: ");
    Serial.println(properties.qos);
    Serial.print("  dup: ");
    Serial.println(properties.dup);
    Serial.print("  retain: ");
    Serial.println(properties.retain);
    Serial.print("  len: ");
    Serial.println(len);
    Serial.print("  index: ");
    Serial.println(index);
    Serial.print("  total: ");
    Serial.println(total);
}

void onMqttPublish(uint16_t packetId)
{
    Serial.println("Publish acknowledged.");
    Serial.print("  packetId: ");
    Serial.println(packetId);
}

void init_mqtt()
{
    Serial.println();
    Serial.println();

    mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
    wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));

    WiFi.onEvent(WiFiEvent);

    mqttClient.onConnect(onMqttConnect);
    mqttClient.onDisconnect(onMqttDisconnect);
    mqttClient.onSubscribe(onMqttSubscribe);
    mqttClient.onUnsubscribe(onMqttUnsubscribe);
    mqttClient.onMessage(onMqttMessage);
    mqttClient.onPublish(onMqttPublish);
    mqttClient.setServer(MQTT_HOST, MQTT_PORT);

    connectToWifi();
}

#endif