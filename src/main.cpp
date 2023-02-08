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
#include "rtc_clock.h"
#include "deep_sleep.h"
#include "mqtt.h"
#include "gps.h"

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

  // init_clock();
  // alarm_set();
  // deep_sleep_init();
  init_mqtt();

  delay(1000);
  
  init_gps();

} // void setup()

void loop()
{
  // check_btn();
  // pwm_loop();
  // clock_loop();
  // if (deep)
  // {
  //   deep = false;
  //   deep_sleep_init();
  // }

  loop_gps();
} // void loop()