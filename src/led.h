#ifndef led_h
#define led_h

struct Led
{
  // state variables
  uint8_t pin;
  bool on;

  // methods
  void update()
  {
    digitalWrite(pin, on ? HIGH : LOW);
  }
};

Led onboard_led = {2, false};

void init_led()
{
  pinMode(onboard_led.pin, OUTPUT);
} //init_led

#endif