#ifndef WIZARD_H
#define WIZARD_H

#include <Preferences.h>
#include <ESPAsyncWebServer.h>

class SetupWizard
{
public:
    SetupWizard();
    void setup();

private:
    const char *ssid = "MoistureMate";
    const char *password = "123456789";
    Preferences preferences;
};

#endif /* WIZARD_H */