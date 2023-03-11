#ifndef sk_led
#define sk_led

#include <NeoPixelBus.h>
const uint8_t PixelCount = 7;
const uint8_t PixelPin = 13;

#define colorSaturation 255

// NeoPixelBus<NeoRgbwFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
NeoPixelBus<NeoGrbwFeature, NeoSk6812Method> strip(PixelCount, PixelPin);

RgbwColor red(colorSaturation, 0, 0, 0);
RgbwColor green(0, colorSaturation, 0, 0);
RgbwColor blue(0, 0, colorSaturation, 0);
RgbwColor white(0, 0, 0, colorSaturation);
RgbwColor whiteC(180, 0, 0, colorSaturation);
RgbwColor Full(colorSaturation,colorSaturation,colorSaturation,colorSaturation);
RgbwColor black(0);


void sk_test()
{
    for (int i = 0; i < PixelCount; i++)
    {
        strip.SetPixelColor(i, red);
    }
    strip.Show();
    delay(500);
    for (int i = 0; i < PixelCount; i++)
    {
        strip.SetPixelColor(i, green);
    }
    strip.Show();
    delay(500);
    for (int i = 0; i < PixelCount; i++)
    {
        strip.SetPixelColor(i, blue);
    }
    strip.Show();
    delay(500);
    for (int i = 0; i < PixelCount; i++)
    {
        strip.SetPixelColor(i, white);
    }
    strip.Show();
    delay(500);
    for (int i = 0; i < PixelCount; i++)
    {
        strip.SetPixelColor(i, black);
    }
    strip.Show();
}

void init_sk()
{
    // this resets all the neopixels to an off state
    strip.Begin();
    strip.Show();
    sk_test();
}
#endif