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

        onboard_led.on = millis() % 400 < 200;
        onboard_led.update();
        digitalWrite(CMD_GPIOPIN, HIGH);
    }
    digitalWrite(CMD_GPIOPIN, LOW);
}

#endif