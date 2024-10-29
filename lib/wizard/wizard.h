#ifndef WIZARD_H
#define WIZARD_H

#include <ESPAsyncWebServer.h>
#include "ConfigManager.h"


class SetupWizard
{
public:
    SetupWizard(ConfigManager &configManager);
    void setup();

private:
    const char *ssid = "MoistureMate";
    const char *password = "123456789";
    ConfigManager configManager;
};

#endif /* WIZARD_H */