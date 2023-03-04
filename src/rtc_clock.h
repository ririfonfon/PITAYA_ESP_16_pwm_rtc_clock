#ifndef rtc_clock
#define rtc_clock

#include <Wire.h> // must be included here so that Arduino library object file references work
#include <RtcDS3231.h>
#include <EepromAT24C32.h>

RtcDS3231<TwoWire> Rtc(Wire);
EepromAt24c32<TwoWire> RtcEeprom(Wire);

// char Wdata[] = "What time is it in Greenwich?";

#define countof(a) (sizeof(a) / sizeof(a[0]))

RtcDateTime old;
RtcDateTime alarmTime;

// char DaysOfWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
char DaysOfWeek[7][12] = {"Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"};

RtcDateTime now = Rtc.GetDateTime();

DS3231AlarmOne alarm_one = Rtc.GetAlarmOne();
RtcDateTime time_on(now.Year(),
                    now.Month(),
                    now.Day(),
                    alarm_one.Hour(),
                    alarm_one.Minute(),
                    alarm_one.Second());
RtcDateTime time_off(now.Year(),
                     now.Month(),
                     now.Day(),
                     time_off_Hour,
                     time_off_Minute,
                     time_off_Second);

void printDateTime(const RtcDateTime &dt)
{
    char datestring[20];

    snprintf_P(datestring,
               countof(datestring),
               PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
               dt.Day(),
               dt.Month(),
               dt.Year(),
               dt.Hour(),
               dt.Minute(),
               dt.Second());
    Serial.print(" -> ");
    Serial.print(DaysOfWeek[dt.DayOfWeek()]);
    Serial.print(" ");
    Serial.print(datestring);
}

void publishGpsTime(TinyGPSTime &t)
{
    char datestring[32];

    snprintf_P(datestring,
               countof(datestring),
               PSTR("%02u:%02u:%02u:%02u:%02u"),
               t.hour(),
               t.minute(),
               t.second(),
               t.centisecond(),
               t.age());
    mqtt_topic = String(MQTT_ID_TIME);
    mqtt_topic.toCharArray(mqtt_topic_char, mqtt_topic.length() + 1);
    mqttClient.publish(mqtt_topic_char, 0, true, datestring);
}

void publishTimeOn(const RtcDateTime &dt)
{
    char datestring[20];

    snprintf_P(datestring,
               countof(datestring),
               PSTR("%02u:%02u:%02u"),
               dt.Hour(),
               dt.Minute(),
               dt.Second());
    mqtt_topic = String(MQTT) + String(ID) + String(MQTT_TIME_ON);
    mqtt_topic.toCharArray(mqtt_topic_char, mqtt_topic.length() + 1);
    mqttClient.publish(mqtt_topic_char, 0, true, datestring);
}

void publishTimeOff(const RtcDateTime &dt)
{
    char datestring[20];

    snprintf_P(datestring,
               countof(datestring),
               PSTR("%02u:%02u:%02u"),
               dt.Hour(),
               dt.Minute(),
               dt.Second());
    mqtt_topic = String(MQTT) + String(ID) + String(MQTT_TIME_OFF);
    mqtt_topic.toCharArray(mqtt_topic_char, mqtt_topic.length() + 1);
    mqttClient.publish(mqtt_topic_char, 0, true, datestring);
}

void init_clock()
{
    //--------RTC SETUP ------------

    Rtc.Begin(22, 21); // the available pins for SDA, SCL
    RtcEeprom.Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

#ifdef DEBUG
    Serial.print("compiled : ");
    Serial.print("\t");
    printDateTime(compiled);
    Serial.println();
#endif

    if (!Rtc.IsDateTimeValid())
    {
        if (Rtc.LastError() != 0)
        {
            // we have a communications error
            // see https://www.arduino.cc/en/Reference/WireEndTransmission for
            // what the number means
            Serial.print("RTC communications error = ");
            Serial.println(Rtc.LastError());
        }
        else
        {
            Serial.println("RTC lost confidence in the DateTime!");
            Rtc.SetDateTime(compiled);
        }
    }

    if (!Rtc.GetIsRunning())
    {
        Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true);
    }

    RtcDateTime now = Rtc.GetDateTime();
    RtcDateTime now_gps = RtcDateTime(gps.date.year(), gps.date.month(), gps.date.day(), gps.time.hour(), gps.time.minute(), gps.time.second());
#ifdef DEBUG
    Serial.print("now : ");
    Serial.print("\t\t");
    printDateTime(now);
    Serial.println();
    Serial.print("now_gps : ");
    Serial.print("\t");
    printDateTime(now_gps);
    Serial.println();

    Serial.print("Pure now : ");
    Serial.print("\t\t");
    Serial.print(now.TotalSeconds());
    Serial.println();
    Serial.print("Pure compiled : ");
    Serial.print("\t");
    Serial.print(compiled.TotalSeconds());
    Serial.println();
#endif

    if (now < compiled)
    {
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        Rtc.SetDateTime(compiled);
    }

    // never assume the Rtc was last configured by you, so
    // just clear them to your needed state
    Rtc.Enable32kHzPin(false);
    Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);

    /* comment out on a second run to see that the info is stored long term */
    // Store something in memory on the Eeprom
}

void compare_clock_gps()
{
    RtcDateTime now = Rtc.GetDateTime();
    RtcDateTime now_gps = RtcDateTime(gps.date.year(), gps.date.month(), gps.date.day(), gps.time.hour(), gps.time.minute(), gps.time.second());
    RtcDateTime now_gps_time_zone = now_gps + (time_zone * 3600);

    if (now != now_gps_time_zone)
    {
        // #ifdef DEBUG
        Serial.print("COMPARE now : ");
        Serial.print("\t\t\t");
        printDateTime(now);
        Serial.println();
        Serial.print("COMPARE now_gps_time_zone : ");
        Serial.print("\t");
        printDateTime(now_gps_time_zone);
        Serial.println();

        Serial.print("COMPARE Pure now : ");
        Serial.print("\t\t\t");
        Serial.print(now.TotalSeconds());
        Serial.println();
        Serial.print("COMPARE Pure now_gps : ");
        Serial.print("\t\t\t");
        Serial.print(now_gps.TotalSeconds());
        Serial.println();
        Serial.print("COMPARE Pure now_gps_time_zone : ");
        Serial.print("\t");
        Serial.print(now_gps_time_zone.TotalSeconds());
        Serial.println();
        Serial.print("COMPARE gps.time.age() : ");
        Serial.print("\t\t\t");
        Serial.print(gps.time.age());
        // #endif

        bool zeroclock = false;
        while (!zeroclock)
        {
            // if ((gps.time.centisecond() - ceil(gps.time.age() / 10)) == 0)
            if (gps.time.centisecond()  == 0)
            {
                RtcDateTime now_gps_time_zone = now_gps + (time_zone * 3600);
                Rtc.SetDateTime(now_gps_time_zone);
                Serial.println();
                Serial.println("RTC != than gps time!  (Updating DateTime)");
                zeroclock = true;
                old = now;
            }
        }
    }
}

void alarm_set()
{

    Rtc.Enable32kHzPin(false);
    Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeAlarmBoth);

    // DS3231AlarmOne alarm1(
    //     01,
    //     18,
    //     18,
    //     00,
    //     DS3231AlarmOneControl_HoursMinutesSecondsMatch);
    // Rtc.SetAlarmOne(alarm1);

    // Alarm 1 set to trigger every day when
    // the hours, minutes, and seconds match

    // alarmTime = Rtc.GetDateTime() + 60; // into the future

    // Serial.print(" next alarm set to ");
    // printDateTime(alarmTime);
    // Serial.println(" ");

    // DS3231AlarmOne alarm1(
    //     alarmTime.Day(),
    //     alarmTime.Hour(),
    //     alarmTime.Minute(),
    //     alarmTime.Second(),
    //     DS3231AlarmOneControl_HoursMinutesSecondsMatch);
    // Rtc.SetAlarmOne(alarm1);

    DS3231AlarmOne alarm1(
        0,
        time_on_Hour,
        time_on_Minute,
        time_on_Second,
        DS3231AlarmOneControl_HoursMinutesSecondsMatch);
    Rtc.SetAlarmOne(alarm1);

    // Alarm 2 set to trigger at the top of the minute

    // DS3231AlarmTwo alarm2(
    //     0,
    //     0,
    //     0,
    //     DS3231AlarmTwoControl_HoursMinutesDayOfMonthMatch);
    // Rtc.SetAlarmTwo(alarm2);

    // throw away any old alarm state before we ran
    // Rtc.LatchAlarmsTriggeredFlags();
}

void loop_clock_mqtt()
{
    now = Rtc.GetDateTime();
    alarm_one = Rtc.GetAlarmOne();
    RtcDateTime time_on(now.Year(),
                        now.Month(),
                        now.Day(),
                        alarm_one.Hour(),
                        alarm_one.Minute(),
                        alarm_one.Second());
    RtcDateTime time_off(now.Year(),
                         now.Month(),
                         now.Day(),
                         time_off_Hour,
                         time_off_Minute,
                         time_off_Second);

    if (!Rtc.IsDateTimeValid())
    {

        if (Rtc.LastError() != 0)
        {
            // we have a communications error
            // see https://www.arduino.cc/en/Reference/WireEndTransmission for
            // what the number means
            Serial.print("RTC loop communications error = ");
            Serial.println(Rtc.LastError());
            init_clock();
        }
        else
        {
            // Common Causes:
            //    1) the battery on the device is low or even missing and the power line was disconnected
            Serial.println("RTC loop lost confidence in the DateTime!");
        }
        return;
    }

    if (old < now - 59)
    {
        old = now;

        // compare_clock_gps();

#ifdef DEBUG
        printDateTime(now);
        Serial.print("      **      time_on : ");
        printDateTime(time_on);
        Serial.print("      **      time_off : ");
        printDateTime(time_off);
        Serial.println();
#endif

        publishTimeOn(time_on);
        publishTimeOff(time_off);
        publishGpsTime(gps.time);
    }

    return;
}

#endif