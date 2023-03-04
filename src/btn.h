#ifndef btn_h
#define btn_h

#include "Arduino.h"

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
        // RtcDateTime now = Rtc.GetDateTime();
        // onboard_led.on = now.Second() % 2;
        onboard_led.on = gps.time.second() % 2;
        onboard_led.update();
        digitalWrite(CMD_GPIOPIN, HIGH);
        smartDelay(0);
    }
    digitalWrite(CMD_GPIOPIN, LOW);
}

#endif