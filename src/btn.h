#ifndef btn_h
#define btn_h

#include "Arduino.h"

// uint8_t l_chose = 0;

void clock_display();

void init_btn()
{
    pinMode(BTN_GPIOPIN, INPUT);
    pinMode(CMD_GPIOPIN, OUTPUT);
    digitalWrite(CMD_GPIOPIN, LOW);
}

void check_btn()
{
    while (digitalRead(BTN_GPIOPIN))
    {
        loop_gps();
        loop_clock_mqtt();
        clock_display();
        // while (compare_count < 11)
        // {
        //     compare_clock_gps();
        // }

        // RtcDateTime now = Rtc.GetDateTime();
        // onboard_led.on = now.Second() % 2;
        onboard_led.on = gps.time.second() % 2;
        // l_chose = gps.time.second() % 7;
        onboard_led.update();
        if (onboard_led.on)
        {
            for (int i = 0; i < PixelCount; i++)
            {
                // strip.SetPixelColor(l_chose, white);
                // strip.SetPixelColor(i, whiteC);
                strip.SetPixelColor(i, Full);
            }
        }
        else
        {
            for (int i = 0; i < PixelCount; i++)
            {
                strip.SetPixelColor(i, black);
            }
        }
        strip.Show();
        digitalWrite(CMD_GPIOPIN, HIGH);
        smartDelay(0);
    }
    digitalWrite(CMD_GPIOPIN, LOW);
}

#endif