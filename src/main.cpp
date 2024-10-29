#include <Arduino.h>
#include "ConfigManager.h"
#include <WiFi.h>
#include <wizard.h>

const char *ssid = "MoistureMate";
const char *password = "123456789";
ConfigManager configManager;
SetupWizard wizard(configManager);

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting...");

  // Load configuration
  if (!configManager.loadConfig())
  {
    Serial.println("Failed to load config");
    wizard.setup();
    return;
  }

  String ssid = configManager.getSSID();
  String password = configManager.getPassword();
  Serial.println("SSID: " + ssid);
  Serial.println("Password: " + password);

  if (ssid.length() == 0 || password.length() == 0)
  {
    Serial.println("Setup wizard started");
    wizard.setup();
  }
  else
  {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(1000);
      Serial.println("Connecting to WiFi...");
    }

    Serial.println("Connected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  }
}

void loop()
{
  Serial.println("Loop");
}