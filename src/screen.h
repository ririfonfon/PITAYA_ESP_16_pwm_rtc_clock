#ifndef screnn_h
#define screen_h

// For a connection via I2C using the Arduino Wire include:
// #include <Wire.h>        // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy: #include "SSD1306.h"

// Initialize the OLED display using Arduino Wire:
// SSD1306Wire display(0x3c, 21, 22); // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h e.g. https://github.com/esp8266/Arduino/blob/master/variants/nodemcu/pins_arduino.h
// SSD1306Wire display(0x3c, SDA, SCL); // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h e.g. https://github.com/esp8266/Arduino/blob/master/variants/nodemcu/pins_arduino.h
SSD1306Wire display(0x3c, 22, 21); // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h e.g. https://github.com/esp8266/Arduino/blob/master/variants/nodemcu/pins_arduino.h
// SSD1306Wire display(0x3c, D3, D5);  // ADDRESS, SDA, SCL  -  If not, they can be specified manually.
// SSD1306Wire display(0x3c, SDA, SCL, GEOMETRY_128_32);  // ADDRESS, SDA, SCL, OLEDDISPLAY_GEOMETRY  -  Extra param required for 128x32 displays.
// SH1106Wire display(0x3c, SDA, SCL);     // ADDRESS, SDA, SCL

void hello_display();

void init_screen()
{
    // Initialising the UI will init the display too.
    display.init();
    display.flipScreenVertically();
    hello_display();
}
void check_display()
{
    if (display_off)
    {
        display.displayOn();
        display_off = false;
        screen_off = millis();
    }
}


void hello_display()
{
    check_display();
    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 10, "Hello");
    display.drawString(64, 27, "The");
    display.drawString(64, 44, "World");
    display.display();
}

void clock_display()
{
    RtcDateTime now = Rtc.GetDateTime();
    RtcDateTime now_gps = RtcDateTime(gps.date.year(), gps.date.month(), gps.date.day(), gps.time.hour(), gps.time.minute(), gps.time.second());
    RtcDateTime now_gps_time_zone = now_gps + (time_zone * 3600);

    if (now != now_gps_time_zone)
    {
        while (compare_count < 11)
        {
            compare_clock_gps();
        }
    }

    // Serial.print(" ");
    // Serial.print(gps.time.second());

    char gpstimestring[20];

    snprintf_P(gpstimestring,
               countof(gpstimestring),
               PSTR("gps: %02u:%02u:%02u"),
               now_gps_time_zone.Hour(),
               now_gps_time_zone.Minute(),
               now_gps_time_zone.Second());

    char rtctimestring[20];

    snprintf_P(rtctimestring,
               countof(rtctimestring),
               PSTR("rtc:  %02u:%02u:%02u"),
               now.Hour(),
               now.Minute(),
               now.Second());

    check_display();
    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 10, gpstimestring);
    display.drawString(64, 27, "");
    display.drawString(64, 44, rtctimestring);
    display.display();
}

void off_screen()
{
    display.displayOff();
    display_off = true;
}

#endif