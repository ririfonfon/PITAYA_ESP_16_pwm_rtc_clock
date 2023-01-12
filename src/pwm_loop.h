#ifndef pwm_loop_h
#define pwm_loop_h

void pwm_loop()
{
    bool fade_clock;
    unsigned long currentMillis = millis();

    if (cycle)
    {
        previousMillis = currentMillis;
        cycle = false;
    } //if (cycle)

    onboard_led.on = millis() % 2000 < 1000;
    onboard_led.update();

    for (int d = 0; d < FOR_PWM_CHANNELS; d++)
    {
        check_btn();
        if (!f_i[d] && !p[d] && currentMillis - previousMillis > (d + 1) * decalage)
        {
            f_i[d] = true;
#ifdef DEBUG
            Serial.print(" f_i[");
            Serial.print(d);
            Serial.print("] = ");
            Serial.print(f_i[d]);
            Serial.print(" at ");
            Serial.println(currentMillis);
#endif
        } //if (!f_i[d] && !p[d] && currentMillis - previousMillis > (d + 1) * decalage)

        if (f_i[d])
        {
            if (fade_in != 0)
            {
                fade_clock = millis() % fade_in < 1;
                if (fade_clock && f_i[d])
                {
                    value[d] += 1;
                }
                if (value[d] > High_value)
                {
                    value[d] = High_value;
                    f_i[d] = false;
                    p[d] = true;
                    currentp[d] = currentMillis;

#ifdef DEBUG
                    Serial.print("fade_in  currentp[");
                    Serial.print(d);
                    Serial.print("] at ");
                    Serial.println(currentp[d]);
#endif
                }
            } // if (fade_in > 0)
            else if (fade_in <= 0)
            {
                value[d] = High_value;
                f_i[d] = false;
                p[d] = true;
                currentp[d] = currentMillis;

#ifdef DEBUG
                Serial.print("in cut currentp[");
                Serial.print(d);
                Serial.print("] at ");
                Serial.println(currentp[d]);
#endif
            } // else if (fade_in <= 0)
        }     //if (f_i[d])

        if (!f_o[d] && !p[FOR_PWM_CHANNELS] && !n[d] && p[d] && currentMillis - currentp[d] > on)
        {
            f_o[d] = true;
#ifdef DEBUG
            Serial.print(" f_o[");
            Serial.print(d);
            Serial.print("] = ");
            Serial.print(f_o[d]);
            Serial.print(" at ");
            Serial.println(currentMillis);
#endif
        } //(!p[FOR_PWM_CHANNELS] && !n[d] && p[d] && currentMillis - currentp[d] > on)

        if (f_o[d])
        {
            if (fade_out != 0)
            {
                fade_clock = millis() % fade_out < 1;
                if (fade_clock && f_o[d])
                {
                    value[d] -= 1;
                }
                if (value[d] < 0)
                {
                    value[d] = 0;
                    f_o[d] = false;
                    n[d] = true;
                    currentp[d] = currentMillis;

#ifdef DEBUG
                    Serial.print("fade_out  currentp[");
                    Serial.print(d);
                    Serial.print("] at ");
                    Serial.println(currentp[d]);
#endif
                }
            } // if (fade_out != 0)
            else if (fade_out <= 0)
            {
                value[d] = 0;
                f_o[d] = false;
                n[d] = true;
                currentp[d] = currentMillis;

#ifdef DEBUG
                Serial.print("out cut currentp[");
                Serial.print(d);
                Serial.print("] at ");
                Serial.println(currentp[d]);
#endif
            } // else if (fade_out <= 0)
        }     //if (f_i[d])

        //             value[d] = 0;
        //             n[d] = true;
        // #ifdef DEBUG
        //             Serial.print(" value[");
        //             Serial.print(d);
        //             Serial.print("] = 0 at ");
        //             Serial.println(currentMillis);
        // #endif

        if (!p[FOR_PWM_CHANNELS] && n[FOR_PWM_CHANNELS - 1])
        {
            p[FOR_PWM_CHANNELS] = true;
            currentp[FOR_PWM_CHANNELS] = currentMillis;
#ifdef DEBUG
            Serial.print("currentp[");
            Serial.print(FOR_PWM_CHANNELS);
            Serial.print("] at ");
            Serial.println(currentp[FOR_PWM_CHANNELS]);
#endif
        }

        if (p[FOR_PWM_CHANNELS] && currentMillis - currentp[FOR_PWM_CHANNELS] > off)
        {
            previousMillis = currentMillis;
            for (int q = 0; q < FOR_PWM_CHANNELS; q++)
            {
                p[q] = false;
                f_i[q] = false;
                f_o[q] = false;
                n[q] = false;
                p[FOR_PWM_CHANNELS] = false;
            }
            rnd();
        } //if (p[FOR_PWM_CHANNELS] && currentMillis - currentp[FOR_PWM_CHANNELS] > off)
    }     //for (int d = 0; d < FOR_PWM_CHANNELS; d++)

    for (int d = 0; d < FOR_PWM_CHANNELS; d++)
    {
        // ledcWrite(d, ((value[ref[d]] * value[ref[d]]) / 65535));
        ledcWrite(d, value[ref[d]]);

    } //for(int d=0; d>FOR_PWM_CHANNELS;d++)
    check_btn();
}

#endif