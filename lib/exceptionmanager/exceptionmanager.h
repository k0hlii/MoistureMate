#ifndef EXCEPTIONMANAGER_H
#define EXCEPTIONMANAGER_H

#include <Adafruit_NeoPixel.h>

#define LED_PIN 33

class ExceptionManager
{
public:
    ExceptionManager();
    void warn();
    void error();
    void stop();

    bool isError = false;
    bool isWarning = false;

private:
    Adafruit_NeoPixel pixels;
};

#endif /* EXCEPTIONMANAGER_H */