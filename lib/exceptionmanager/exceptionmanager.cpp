#include <exceptionmanager.h>
#include <Adafruit_NeoPixel.h>

ExceptionManager::ExceptionManager(/* args */)
{
    pixels = Adafruit_NeoPixel(1, LED_PIN, NEO_GRB + NEO_KHZ800);
    pixels.begin();
}

void ExceptionManager::warn()
{
    isWarning = true;

    pixels.setPixelColor(0, pixels.Color(255, 255, 0)); // yellow
    pixels.show();
}

void ExceptionManager::error()
{
    isError = true;
    pixels.setPixelColor(0, pixels.Color(255, 0, 0)); // red
    pixels.show();
}

void ExceptionManager::stop()
{
    isError = false;
    isWarning = false;
    pixels.clear();
}
