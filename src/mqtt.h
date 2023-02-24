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

char mqtt_topic_char[64];
char mqtt_topic_char_id[8];
char mqtt_topic_char_set_time_on[32];
char mqtt_topic_char_set_time_off[32];
char mqtt_topic_char_set_alt_coef[32];
char mqtt_topic_char_set_lat_coef[32];
char mqtt_topic_char_set_long_coef[32];

String mqtt_topic;
String MQTT = "gps/";
String MQTT_SAT = "/sat";
String MQTT_ALT = "/alt";
String MQTT_LONG = "/long";
String MQTT_LAT = "/lat";
String MQTT_TIME_ON = "/timeon";
String MQTT_TIME_OFF = "/timeoff";

String MQTT_ID = String(MQTT) + String(ID);
String MQTT_SET_TIME_ON = String (MQTT_ID) + "/set/time_on";
String MQTT_SET_TIME_OFF = String (MQTT_ID) + "/set/time_off";
String MQTT_SET_ALT_COEF = String (MQTT_ID) + "/set/alt_coef";
String MQTT_SET_LAT_COEF = String (MQTT_ID) + "/set/lat_coef";
String MQTT_SET_LONG_COEF = String (MQTT_ID) + "/set/long_coef";

boolean wifihasFix = false;
void connectToWifi()
{
#ifdef DEBUG
    Serial.println("Connecting to Wi-Fi...");
#endif
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (!wifihasFix)
    {
#ifdef DEBUG
        Serial.print(".");
#endif
        delay(1000);
    }
}

void connectToMqtt()
{
#ifdef DEBUG
    Serial.println("Connecting to MQTT...");
#endif
    mqttClient.connect();
}

void WiFiEvent(WiFiEvent_t event)
{
#ifdef DEBUG
    Serial.printf("[WiFi-event] event: %d\n", event);
#endif
    switch (event)
    {
    case SYSTEM_EVENT_STA_GOT_IP:
#ifdef DEBUG
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
#endif
        wifihasFix = true;
        connectToMqtt();
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
#ifdef DEBUG
        Serial.println("WiFi lost connection");
#endif
        xTimerStop(mqttReconnectTimer, 0); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
        xTimerStart(wifiReconnectTimer, 0);
        break;
    }
}

void onMqttConnect(bool sessionPresent)
{
#ifdef DEBUG
    Serial.println("Connected to MQTT.");
    Serial.print("Session present: ");
    Serial.println(sessionPresent);
#endif
    MQTT_ID.toCharArray(mqtt_topic_char, MQTT_ID.length() + 1);
    uint16_t packetIdSub = mqttClient.subscribe(mqtt_topic_char, 2);
#ifdef DEBUG
    Serial.print("Subscribing at QoS 2, packetId: ");
    Serial.print(packetIdSub);
    Serial.print(" TOPIC : ");
    Serial.println(mqtt_topic_char);
#endif
    mqtt_topic = String(MQTT) + String("all");
    mqtt_topic.toCharArray(mqtt_topic_char, mqtt_topic.length() + 1);
    uint16_t packetIdSub2 = mqttClient.subscribe(mqtt_topic_char, 2);
#ifdef DEBUG
    Serial.print("Subscribing at QoS 2, packetId: ");
    Serial.print(packetIdSub2);
    Serial.print(" TOPIC : ");
    Serial.println(mqtt_topic_char);
#endif
    MQTT_SET_TIME_ON.toCharArray(mqtt_topic_char, MQTT_SET_TIME_ON.length() + 1);
    uint16_t packetIdSub3 = mqttClient.subscribe(mqtt_topic_char, 2);
#ifdef DEBUG
    Serial.print("Subscribing at QoS 2, packetId: ");
    Serial.print(packetIdSub3);
    Serial.print(" TOPIC : ");
    Serial.println(mqtt_topic_char);
#endif
    MQTT_SET_TIME_OFF.toCharArray(mqtt_topic_char, MQTT_SET_TIME_OFF.length() + 1);
    uint16_t packetIdSub4 = mqttClient.subscribe(mqtt_topic_char, 2);
#ifdef DEBUG
    Serial.print("Subscribing at QoS 2, packetId: ");
    Serial.print(packetIdSub4);
    Serial.print(" TOPIC : ");
    Serial.println(mqtt_topic_char);
#endif
    MQTT_SET_ALT_COEF.toCharArray(mqtt_topic_char, MQTT_SET_ALT_COEF.length() + 1);
    uint16_t packetIdSub5 = mqttClient.subscribe(mqtt_topic_char, 2);
#ifdef DEBUG
    Serial.print("Subscribing at QoS 2, packetId: ");
    Serial.print(packetIdSub5);
    Serial.print(" TOPIC : ");
    Serial.println(mqtt_topic_char);
#endif
    MQTT_SET_LAT_COEF.toCharArray(mqtt_topic_char, MQTT_SET_LAT_COEF.length() + 1);
    uint16_t packetIdSub6 = mqttClient.subscribe(mqtt_topic_char, 2);
#ifdef DEBUG
    Serial.print("Subscribing at QoS 2, packetId: ");
    Serial.print(packetIdSub6);
    Serial.print(" TOPIC : ");
    Serial.println(mqtt_topic_char);
#endif
    MQTT_SET_LONG_COEF.toCharArray(mqtt_topic_char, MQTT_SET_LONG_COEF.length() + 1);
    uint16_t packetIdSub7 = mqttClient.subscribe(mqtt_topic_char, 2);
#ifdef DEBUG
    Serial.print("Subscribing at QoS 2, packetId: ");
    Serial.print(packetIdSub7);
    Serial.print(" TOPIC : ");
    Serial.println(mqtt_topic_char);
#endif


}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
{
#ifdef DEBUG
    Serial.println("Disconnected from MQTT.");
#endif

    if (WiFi.isConnected())
    {
        xTimerStart(mqttReconnectTimer, 0);
    }
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos)
{
#ifdef DEBUG
    Serial.print("Subscribe acknowledged.");
    Serial.print("  packetId: ");
    Serial.print(packetId);
    Serial.print("  qos: ");
    Serial.println(qos);
#endif
}

void onMqttUnsubscribe(uint16_t packetId)
{
#ifdef DEBUG
    Serial.println("Unsubscribe acknowledged.");
    Serial.print("  packetId: ");
    Serial.println(packetId);
#endif
}

void onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
    MQTT_ID.toCharArray(mqtt_topic_char_id, MQTT_ID.length() + 1);
    MQTT_SET_TIME_ON.toCharArray(mqtt_topic_char_set_time_on, MQTT_ID.length() + 1);
    MQTT_SET_TIME_OFF.toCharArray(mqtt_topic_char_set_time_off, MQTT_ID.length() + 1);
    MQTT_SET_ALT_COEF.toCharArray(mqtt_topic_char_set_alt_coef, MQTT_ID.length() + 1);
    MQTT_SET_LAT_COEF.toCharArray(mqtt_topic_char_set_lat_coef, MQTT_ID.length() + 1);
    MQTT_SET_LONG_COEF.toCharArray(mqtt_topic_char_set_long_coef, MQTT_ID.length() + 1);
    // Serial.print("MQTT_SET_TIME_ON :");
    // Serial.println(MQTT_SET_TIME_ON);
    // Serial.print("MQTT_SET_TIME_OFF :");
    // Serial.println(MQTT_SET_TIME_OFF);
    // Serial.print("MQTT_SET_ALT_COEF :");
    // Serial.println(MQTT_SET_ALT_COEF);
    // Serial.print("MQTT_SET_LAT_COEF :");
    // Serial.println(MQTT_SET_LAT_COEF);
    // Serial.print("MQTT_SET_LONG_COEF :");
    // Serial.println(MQTT_SET_LONG_COEF);
#ifdef DEBUG
    Serial.print("Publish received. ");
    Serial.print("  topic: ");
    Serial.print(topic);
    Serial.print("  qos: ");
    Serial.print(properties.qos);
    Serial.print("  dup: ");
    Serial.print(properties.dup);
    Serial.print("  retain: ");
    Serial.print(properties.retain);
    Serial.print("  len: ");
    Serial.print(len);
    Serial.print("  index: ");
    Serial.print(index);
    Serial.print("  total: ");
    Serial.println(total);
#endif
    if (strcmp(topic, "gps/all") == 0)
    {
#ifdef DEBUG
        Serial.print("gps/all  ");
        Serial.print("  raconte: ");
        Serial.println(payload);
#endif
    }
    else if (strcmp(topic, mqtt_topic_char_id) == 0)
    {
#ifdef DEBUG
        Serial.print(MQTT_ID);
        Serial.print("  raconte: ");
        Serial.println(payload);
#endif
    }
    else if (strcmp(topic, "gps/12/set/time_on") == 0)
    // else if (strcmp(topic, mqtt_topic_char_set_time_on) == 0)
    {
#ifdef DEBUG
        Serial.print(MQTT_SET_TIME_ON);
        Serial.print("  raconte: ");
        Serial.println(payload);
#endif
    }
    else if (strcmp(topic, mqtt_topic_char_set_time_off) == 0)
    {
#ifdef DEBUG
        Serial.print(MQTT_SET_TIME_OFF);
        Serial.print("  raconte: ");
        Serial.println(payload);
#endif
    }
    else if (strcmp(topic, mqtt_topic_char_set_alt_coef) == 0)
    {
#ifdef DEBUG
        Serial.print(MQTT_SET_ALT_COEF);
        Serial.print("  raconte: ");
        Serial.println(payload);
#endif
    }
    else if (strcmp(topic, mqtt_topic_char_set_lat_coef) == 0)
    {
#ifdef DEBUG
        Serial.print(MQTT_SET_LAT_COEF);
        Serial.print("  raconte: ");
        Serial.println(payload);
#endif
    }
    else if (strcmp(topic, mqtt_topic_char_set_long_coef) == 0)
    {
#ifdef DEBUG
        Serial.print(MQTT_SET_LONG_COEF);
        Serial.print("  raconte: ");
        Serial.println(payload);
#endif
    }
}

void onMqttPublish(uint16_t packetId)
{
#ifdef DEBUG
    Serial.println("Publish acknowledged.");
    Serial.print("  packetId: ");
    Serial.println(packetId);
#endif
}

void init_mqtt()
{
#ifdef DEBUG
    Serial.println();
    Serial.println();
#endif

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