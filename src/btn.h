#ifndef btn_h
#define btn_h

#include "Arduino.h"

void init_btn()
{
    pinMode(BTN_GPIOPIN, INPUT);
    pinMode(CMD_GPIOPIN,OUTPUT);
    digitalWrite(CMD_GPIOPIN,LOW);
}

void check_btn()
{
    btn_State = digitalRead(BTN_GPIOPIN);

    if (btn_State == HIGH)
    {
        digitalWrite(CMD_GPIOPIN,HIGH);
#ifdef DEBUG
        Serial.print("(btn_State == HIGH)");
#endif
    }
    while (btn_State)
    {
        loop_gps();
        loop_clock_mqtt();

        onboard_led.on = millis() % 400 < 200;
        onboard_led.update();
    }
    digitalWrite(CMD_GPIOPIN,LOW);
}

#endif