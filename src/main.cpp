#include <Arduino.h>
#include <Preferences.h>
#include <wizard.h>

Preferences preferences;

void setup()
{
  Serial.begin(9600);
  if (!preferences.begin("my-app", false))
  {
    Serial.println("Failed to initialize NVS");
    return;
  }
  else
  {
    Serial.println("NVS initialized successfully");
  }
  
  // Reinitialize WiFi
  String ssid = preferences.getString("ssid","");
  String password = preferences.getString("password","");
  Serial.println("SSID: " + ssid);
  Serial.println("Password: " + password);

  if (ssid.length() == 0)
  {
    SetupWizard wizard;
    Serial.println("Setup wizard started");
    wizard.setup();
  }
  else
  {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    int retries = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
      if (retries > 5)
      {
        WiFi.disconnect();
        Serial.println("Failed to connect to WiFi");
        SetupWizard wizard;
        Serial.println("Setup wizard started");
        wizard.setup();
        break;
      }
      delay(500);
      Serial.println("Connecting to WiFi...");
      retries++;
    }
  }
}

void loop()
{
  // auto now = millis();
  // auto delta = now - time1;
  // if(delta > 1000) {
  //   Serial.println("10 seconds passed");
  //   time1 = now;
  //   Serial.println(WiFi.status());
  // }
  String ssid = preferences.getString("ssid","");
  String password = preferences.getString("password","");
  Serial.println("SSID: " + ssid);
  Serial.println("Password: " + password);
}
