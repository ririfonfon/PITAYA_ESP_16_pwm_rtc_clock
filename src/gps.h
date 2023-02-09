#ifndef gps_tool
#define gps_tool

#include <Arduino.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <RtcDS3231.h>
/*
   This sample code demonstrates the normal use of a TinyGPSPlus (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
static const int RXPin = 16, TXPin = 17;
static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

static void printDateTime(TinyGPSDate &d, TinyGPSTime &t);
static void smartDelay(unsigned long ms);
// static void GpsGetDateTime(TinyGPSDate &d, TinyGPSTime &t);

static void publishSat(unsigned long val, bool valid, int len);
static void publishLat(float val, bool valid, int len, int prec);
static void publishAlt(float val, bool valid, int len, int prec);
static void publishLong(float val, bool valid, int len, int prec);

void init_gps()
{
    ss.begin(GPSBaud);
}

void loop_gps()
{
    if (gps.location.isUpdated())
    {
    publishLat(gps.location.lat(), gps.location.isValid(), 11, 6);
    publishLong(gps.location.lng(), gps.location.isValid(), 12, 6);
    }
    if (gps.altitude.isUpdated())
    {
    publishAlt(gps.altitude.meters(), gps.altitude.isValid(), 7, 2);
    }
    if (gps.satellites.isUpdated())
    {
    publishSat(gps.satellites.value(), gps.satellites.isValid(), 5);
    }

    // printDateTime(gps.date, gps.time);

    smartDelay(1000);

    if (millis() > 5000 && gps.charsProcessed() < 10)
        Serial.println(F("No GPS data received: check wiring"));
}

// This custom version of delay() ensures that the gps object
// is being "fed".
static void smartDelay(unsigned long ms)
{
    unsigned long start = millis();
    do
    {
        while (ss.available())
            gps.encode(ss.read());
    } while (millis() - start < ms);
}

static void publishLat(float val, bool valid, int len, int prec)
{
    mqtt_topic = String(MQTT) + String(ID) + String(MQTT_LAT);
    mqtt_topic.toCharArray(MQTT_TOPIC, mqtt_topic.length() + 1);
    char sz[32] = "*****************";
    if (!valid)
    {
    }
    else
    {
        int val_int = (int)val;
        float val_float = (abs(val) - abs(val_int)) * 1000000;
        int val_fra = (int)val_float;
        sprintf(sz, "%d.%d", val_int, val_fra);
    }
    mqttClient.publish(MQTT_TOPIC, 0, true, sz);
    smartDelay(0);
}

static void publishLong(float val, bool valid, int len, int prec)
{
    mqtt_topic = String(MQTT) + String(ID) + String(MQTT_LONG);
    mqtt_topic.toCharArray(MQTT_TOPIC, mqtt_topic.length() + 1);
    char sz[32] = "*****************";
    if (!valid)
    {
    }
    else
    {
        int val_int = (int)val;
        float val_float = (abs(val) - abs(val_int)) * 1000000;
        int val_fra = (int)val_float;
        sprintf(sz, "%d.%d", val_int, val_fra);
    }
    mqttClient.publish(MQTT_TOPIC, 0, true, sz);
    smartDelay(0);
}

static void publishAlt(float val, bool valid, int len, int prec)
{
    mqtt_topic = String(MQTT) + String(ID) + String(MQTT_ALT);
    mqtt_topic.toCharArray(MQTT_TOPIC, mqtt_topic.length() + 1);
    char sz[32] = "*****************";
    if (!valid)
    {
    }
    else
    {
        int val_int = (int)val;
        float val_float = (abs(val) - abs(val_int)) * 1000000;
        int val_fra = (int)val_float;
        sprintf(sz, "%d.%d", val_int, val_fra);
    }
    mqttClient.publish(MQTT_TOPIC, 0, true, sz);
    smartDelay(0);
}

static void publishSat(unsigned long val, bool valid, int len)
{
    char sz[32] = "*****************";
    if (valid)
        sprintf(sz, "%ld", val);
    sz[len] = 0;
    for (int i = strlen(sz); i < len; ++i)
        sz[i] = ' ';
    if (len > 0)
        sz[len - 1] = ' ';
    mqtt_topic = String(MQTT) + String(ID) + String(MQTT_SAT);
    mqtt_topic.toCharArray(MQTT_TOPIC, mqtt_topic.length() + 1);
    mqttClient.publish(MQTT_TOPIC, 0, true, sz);
    smartDelay(0);
}

static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
{
    if (!d.isValid())
    {
        Serial.print(F("********** "));
    }
    else
    {
        char sz[32];
        sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
        Serial.print(sz);
    }

    if (!t.isValid())
    {
        Serial.print(F("******** "));
    }
    else
    {
        char sz[32];
        sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
        Serial.print(sz);
    }

    // printInt(d.age(), d.isValid(), 5);
    smartDelay(0);
}

// static void GpsGetDateTime(TinyGPSDate &d, TinyGPSTime &t)
// {
//     uint16_t year = d.year();
//     uint8_t month = d.month();
//     uint8_t dayOfMonth = d.day();
//     uint8_t hour = t.hour();
//     uint8_t minute = t.minute();
//     uint8_t second = t.second();

//     return RtcDateTime(year, month, dayOfMonth, hour, minute, second);
// }

#endif