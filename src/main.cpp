#include <Arduino.h>

#define DEBUG 1
#define ID 12

#include "variable.h"
#include "led.h"
#include "random.h"
#include "mqtt.h"
#include "gps.h"
#include "rtc_clock.h"
#include "eeprom_fonc.h"
#include "deep_sleep.h"
#include "btn.h"
// #include "pwm_loop.h"
#include "screen.h"

void setup()
{
// init serial
#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("Hello World!");
#endif

  // SCREEN
  init_screen();
  Serial.println("Screen ");

  // MQTT
  init_mqtt();
  Serial.println("MQTT");
  delay(3000);

  // GPS
  init_gps();
  Serial.println("GPS");

  // RTC
  init_clock();
  Serial.println("CLOCK");

  // EEPROM
  EEPROM.begin(EEPROM_SIZE);
  Serial.println("EEPROM");
  init_eeprom();
  Serial.println("init_eeprom");

  // init led
  init_led();
  Serial.println("led");

  // init btn
  init_btn();
  Serial.println("btn");

  //   // init pwm
  //   for (int k = 0; k < PWM_CHANNELS; k++)
  //   {
  //     ledcAttachPin(PWM_GPIOPIN[k], k);
  //     ledcSetup(k, PWM_FREQUENCY, PWM_RESOLUTION);
  //   }

  //   // random output
  //   rnd();
  // #ifdef DEBUG
  //   for (int k = 0; k > FOR_PWM_CHANNELS; k++)
  //   {
  //     Serial.println(ref[k]);
  //   }
  // #endif

  //   init_wifi();
  //   WiFi.mode(WIFI_OFF);

  boolean hasFix = false;
  while (!hasFix)
  {
    while (!gps.time.isValid())
    {
      Serial.print("*");
      smartDelay(1000);

      if (millis() > 5000 && gps.charsProcessed() < 10)
        Serial.println(F("No GPS data received: check wiring"));
    }
    if (gps.time.isValid() && gps.date.isValid() && (gps.date.year() != 2000))
    {
      Serial.print("GPS Time OK ");
      printDateTime(gps.date, gps.time);
      Serial.println();
      compare_clock_gps();
      hasFix = true;
    }
    if (gps.time.isValid() && !gps.date.isValid())
    {
      Serial.print("GPS  date NO OK ");
      printDateTime(gps.date, gps.time);
      smartDelay(1000);
    }
    if (gps.time.isValid() && gps.date.isValid() && (gps.date.year() == 2000))
    {
      Serial.print("GPS date 2000 ");
      printDateTime(gps.date, gps.time);
      smartDelay(1000);
    }
  }

  // alarm_set();
  // deep_sleep_init();

} // void setup()

void loop()
{

  check_btn();
  // pwm_loop();
  // if (deep)
  // {
  //   deep = false;
  //   deep_sleep_init();
  // }

} // void loop()