#include <Arduino.h>
#include "ConfigManager.h"
#include <WiFi.h>
#include <wizard.h>
#include <HTTPClient.h>
#include <exceptionmanager.h>

#define SENSOR_PIN 32

ConfigManager configManager;
SetupWizard wizard(configManager);
ExceptionManager exceptionManager;

String ssid;
String password;
int threshold = 2000;

void connectToWifi(String ssid, String password)
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());

  int cnt = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    if (cnt >= 10)
    {
      return;
    }
    delay(1000);
    Serial.println("Connecting to WiFi...");
    cnt++;
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.print("IP Address: ");
    Serial.println("Connected to WiFi");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("Not Connected to WiFi");
    exceptionManager.warn();
  }
}

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("Starting...");

  // Load configuration
  if (!configManager.loadConfig())
  {
    Serial.println("Failed to load config");
    wizard.setup();
    return;
  }

  ssid = configManager.getSSID();
  password = configManager.getPassword();

  Serial.println("SSID: " + ssid);
  Serial.println("Password: " + password);

  if (ssid.length() == 0)
  {
    Serial.println("Setup wizard started");
    wizard.setup();
  }
  else
  {
    connectToWifi(ssid, password);
  }
}

void sendNtfyMessage(const String &message)
{
  String topic = configManager.getTopic();
  String server = "https://" + configManager.getURL();

  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;
    String url = server + topic;
    http.begin(url);
    http.addHeader("Content-Type", "text/plain");

    int httpResponseCode = http.POST(message);

    if (httpResponseCode > 0)
    {
      String response = http.getString();
      Serial.println("HTTP Response code: " + String(httpResponseCode));
      Serial.println("Response: " + response);
      exceptionManager.stop();
    }
    else
    {
      Serial.println("Error on sending POST: " + String(httpResponseCode));
      exceptionManager.error();
    }

    http.end();
  }
  else
  {
    Serial.println("WiFi not connected");
    exceptionManager.warn();
  }
}

void loop()
{
  int sensorValue = analogRead(SENSOR_PIN);
  // Serial.println(sensorValue);
  if (sensorValue > threshold)
  {
    // sendNtfyMessage(configManager.getName() + " ist zu trocken!");
  }

  if (exceptionManager.isError || exceptionManager.isWarning)
  {

  }
  else
  {
    WiFi.disconnect(true);
    exceptionManager.stop();
    Serial.println("Going to sleep...");
    //  ESP.deepSleep(1.08e10); //sleep for 3 hours
    ESP.deepSleep(1e7); // sleep for 10 seconds
  }
}