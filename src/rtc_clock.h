#ifndef rtc_clock
#define rtc_clock

#include <Wire.h> // must be included here so that Arduino library object file references work
#include <RtcDS3231.h>
#include <EepromAT24C32.h>

RtcDS3231<TwoWire> Rtc(Wire);
EepromAt24c32<TwoWire> RtcEeprom(Wire);

const char data[] = "What time is it in Greenwich?";
const uint16_t stringAddr = 64; // stored on page boundary

#define countof(a) (sizeof(a) / sizeof(a[0]))

RtcDateTime old;
RtcDateTime alarmTime;

void printDateTime(const RtcDateTime &dt)
{
    char datestring[20];

    snprintf_P(datestring,
               countof(datestring),
               PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
               dt.Month(),
               dt.Day(),
               dt.Year(),
               dt.Hour(),
               dt.Minute(),
               dt.Second());
    Serial.print(datestring);
}

void init_clock()
{

    Serial.print("compiled: ");
    Serial.print(__DATE__);
    Serial.println(__TIME__);

    //--------RTC SETUP ------------
    // if you are using ESP-01 then uncomment the line below to reset the pins to
    // the available pins for SDA, SCL
    // Wire.begin(0, 2); // due to limited pins, use pin 0 and 2 for SDA, SCL

    Rtc.Begin(22, 21);
    RtcEeprom.Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

    printDateTime(compiled);
    Serial.println();

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

    // store starting address of string
    RtcEeprom.SetMemory(0, stringAddr);
    // store the string, nothing longer than 32 bytes due to paging
    uint8_t written = RtcEeprom.SetMemory(stringAddr, (const uint8_t *)data, sizeof(data) - 1); // remove the null terminator strings add
    // store the length of the string
    RtcEeprom.SetMemory(1, written); // store the
    /* end of comment out section */
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
    alarmTime = Rtc.GetDateTime() + 60; // into the future
    DS3231AlarmOne alarm1(
        alarmTime.Day(),
        alarmTime.Hour(),
        alarmTime.Minute(),
        alarmTime.Second(),
        DS3231AlarmOneControl_HoursMinutesSecondsMatch);
    Rtc.SetAlarmOne(alarm1);

    // Alarm 2 set to trigger at the top of the minute
    DS3231AlarmTwo alarm2(
        0,
        0,
        0,
        DS3231AlarmTwoControl_OncePerMinute);
    Rtc.SetAlarmTwo(alarm2);

    // throw away any old alarm state before we ran
    Rtc.LatchAlarmsTriggeredFlags();
}

void clock_loop()
{

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
            // Common Causes:
            //    1) the battery on the device is low or even missing and the power line was disconnected
            Serial.println("RTC lost confidence in the DateTime!");
        }
        return;
    }

    RtcDateTime now = Rtc.GetDateTime();

    if (old != now)
    {
        old = now;

        printDateTime(now);
        Serial.println();

        // read data

        // get the offset we stored our data from address zero
        uint8_t address = RtcEeprom.GetMemory(0);
        if (address != stringAddr)
        {
            Serial.print("address didn't match ");
            Serial.println(address);
        }
#ifdef clock_eeprom_debug
        {
            // get the size of the data from address 1
            uint8_t count = RtcEeprom.GetMemory(1);
            uint8_t buff[64];

            // get our data from the address with the given size
            uint8_t gotten = RtcEeprom.GetMemory(address, buff, count);

            if (gotten != count ||
                count != sizeof(data) - 1) // remove the extra null terminator strings add
            {
                Serial.print("something didn't match, count = ");
                Serial.print(count, DEC);
                Serial.print(", gotten = ");
                Serial.print(gotten, DEC);
                Serial.println();
            }
            Serial.print("data read (");
            Serial.print(gotten);
            Serial.print(") = \"");
            for (uint8_t ch = 0; ch < gotten; ch++)
            {
                Serial.print((char)buff[ch]);
            }
            Serial.println("\"");
        }
#endif
    }

    if ( alarmTime < Rtc.GetDateTime() + 15 )
    {
        alarm_set();
    }

    return;
}

#endif