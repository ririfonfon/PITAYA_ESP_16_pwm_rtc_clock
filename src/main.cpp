// platformio run --target uploadfs // cmd console download spiff

#include <Arduino.h>

#define DEBUG 1
#define ID 12

#include "variable.h"
#include "led.h"
#include "random.h"
#include "wifi_serv_setup.h"
#include "btn.h"
#include "pwm_loop.h"
#include "mqtt.h"
#include "gps.h"
#include "rtc_clock.h"
#include "deep_sleep.h"

void setup()
{
// init serial
#ifdef DEBUG
  Serial.begin(115200);
  Serial.println();
#endif

  //   // EEPROM
  //   EEPROM.begin(EEPROM_SIZE);
  //   init_eeprom();

  //   // init led
  //   init_led();

  //   // init btn
  //   init_btn();

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

  init_mqtt();

  init_gps();

  boolean hasFix = false;
  while (!hasFix)
  {
    while (!gps.time.isValid())
    {
      Serial.print(".");
      smartDelay(1000);

      if (millis() > 5000 && gps.charsProcessed() < 10)
        Serial.println(F("No GPS data received: check wiring"));
    }
    if (gps.time.isValid() && gps.date.isValid())
    {
      Serial.println("GPS Time OK ");
      printDateTime(gps.date, gps.time);
      hasFix = true;
    }
  }

  init_clock();

  // alarm_set();
  // deep_sleep_init();

} // void setup()

void loop()
{
  loop_gps();
  loop_clock_mqtt();

  // check_btn();
  // pwm_loop();
  // if (deep)
  // {
  //   deep = false;
  //   deep_sleep_init();
  // }

} // void loop()