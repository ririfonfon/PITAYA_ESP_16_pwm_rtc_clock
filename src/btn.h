#ifndef btn_h
#define btn_h

#include "Arduino.h"

const int LONG_PRESS_TIME = 1000;
const int RESET_PRESS_TIME = 10000;
unsigned long pressedTime = 0;
unsigned long releasedTime = 0;
int lastState = LOW;
int currentState;
int boucle = 0;

// uint8_t l_chose = 0;

void clock_display();

void init_btn()
{
    pinMode(BTN_GPIOPIN2, INPUT);
    pinMode(BTN_GPIOPIN, INPUT);
    pinMode(CMD_GPIOPIN, OUTPUT);
    digitalWrite(CMD_GPIOPIN, LOW);
}

void check_btn()
{

    while (digitalRead(BTN_GPIOPIN))
    {
        currentState = digitalRead(BTN_GPIOPIN2);

        if (lastState == LOW && currentState == HIGH) // button is pressed
        {
            pressedTime = millis();
            lastState = currentState;
        }
        if (currentState == HIGH)
        {
            Serial.print("btn press");
            if (millis() - pressedTime > RESET_PRESS_TIME)
            {
            }
            else if (millis() - pressedTime > LONG_PRESS_TIME)
            {
            }
            else if (millis() - pressedTime < LONG_PRESS_TIME)
            {
            }
        }
        else if (lastState == HIGH && currentState == LOW) // button is released
        {
            boucle++;
            if (boucle > 7)
            {
                boucle = 0;
            }
            lastState = currentState;
        }
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
                if (boucle == 0)
                {
                    strip.SetPixelColor(i, red);
                }
                else if (boucle == 1)
                {
                    strip.SetPixelColor(i, green);
                }
                else if (boucle == 2)
                {
                    strip.SetPixelColor(i, blue);
                }
                else if (boucle == 3)
                {
                    strip.SetPixelColor(i, white);
                }
                else if (boucle == 4)
                {
                    strip.SetPixelColor(i, whiteR);
                }
                else if (boucle == 5)
                {
                    strip.SetPixelColor(i, whiteG);
                }
                else if (boucle == 6)
                {
                    strip.SetPixelColor(i, whiteB);
                }
                else if (boucle == 7)
                {
                    strip.SetPixelColor(i, Full);
                }
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

     while (!digitalRead(BTN_GPIOPIN))
    {
        currentState = digitalRead(BTN_GPIOPIN2);

        if (lastState == LOW && currentState == HIGH) // button is pressed
        {
            pressedTime = millis();
            lastState = currentState;
        }
        if (currentState == HIGH)
        {
            // Serial.print("btn press");
            if (millis() - pressedTime > RESET_PRESS_TIME)
            {
            }
            else if (millis() - pressedTime > LONG_PRESS_TIME)
            {
            }
            else if (millis() - pressedTime < LONG_PRESS_TIME)
            {
            }
        }
        else if (lastState == HIGH && currentState == LOW) // button is released
        {
            boucle++;
            if (boucle > 7)
            {
                boucle = 0;
            }
            lastState = currentState;
        }
        loop_gps();
        loop_clock_mqtt();
        clock_display();
        // while (compare_count < 11)
        // {
        //     compare_clock_gps();
        // }

        // RtcDateTime now = Rtc.GetDateTime();
        // onboard_led.on = now.Second() % 2;
        // onboard_led.on = gps.time.second() % 2;
        onboard_led.on = millis() % 2000 < 1000;
        onboard_led.update();
        if (onboard_led.on)
        {
            for (int i = 0; i < PixelCount; i++)
            {
                if (boucle == 0)
                {
                    strip.SetPixelColor(i, red);
                }
                else if (boucle == 1)
                {
                    strip.SetPixelColor(i, green);
                }
                else if (boucle == 2)
                {
                    strip.SetPixelColor(i, blue);
                }
                else if (boucle == 3)
                {
                    strip.SetPixelColor(i, white);
                }
                else if (boucle == 4)
                {
                    strip.SetPixelColor(i, whiteR);
                }
                else if (boucle == 5)
                {
                    strip.SetPixelColor(i, whiteG);
                }
                else if (boucle == 6)
                {
                    strip.SetPixelColor(i, whiteB);
                }
                else if (boucle == 7)
                {
                    strip.SetPixelColor(i, Full);
                }
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
    digitalWrite(CMD_GPIOPIN, HIGH);
}

#endif