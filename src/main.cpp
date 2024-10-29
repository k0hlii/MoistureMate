#include <Arduino.h>
#include "SPIFFS.h"
#include <ESPAsyncWebServer.h>

AsyncWebServer server{80};
const char *ssid = "MoistureMate";
const char *password = "123456789";

void init()
{
  Serial.begin(115200); // Ensure Serial is initialized
  WiFi.softAP(ssid, password);
  Serial.println("AP started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Serve the index.html file
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html", "text/html"); });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/style.css", "text/css"); });

  server.on("/complete", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/complete.html", "text/html"); });

  // Handle data received from the client
  server.on("/submit", HTTP_POST, [](AsyncWebServerRequest *request)
            {
        String ssid, password, url, topic, name, ct;
        if (request->hasParam("ssid", true) && request->hasParam("password", true) &&
            request->hasParam("url", true) && request->hasParam("topic", true) &&
            request->hasParam("name", true) && request->hasParam("ct", true)) {

            ssid = request->getParam("ssid", true)->value();
            password = request->getParam("password", true)->value();
            url = request->getParam("url", true)->value();
            topic = request->getParam("topic", true)->value();
            name = request->getParam("name", true)->value();
            ct = request->getParam("ct", true)->value();

            Serial.println("Received data:");
            Serial.println("SSID: " + ssid);
            Serial.println("Password: " + password);
            Serial.println("Server URL: " + url);
            Serial.println("Topic: " + topic);
            Serial.println("Name: " + name);
            Serial.println("Custom threshold: " + ct);

            // Store the data in SPIFFS
            File file = SPIFFS.open("/config.txt", "w");
            if (!file)
            {
                Serial.println("Failed to open config file for writing");
                return;
            }
            file.println(ssid);
            file.println(password);
            file.println(url);
            file.println(topic);
            file.println(name);
            file.println(ct);
            file.close();

            Serial.println("Data stored in SPIFFS");

            // Send a redirect response
            request->send(200, "text/html", "<html><body><script>window.location.href = '/complete';</script></body></html>");

            delay(1000);

            // Disconnect the AP WiFi
            WiFi.softAPdisconnect(true);
            Serial.println("AP WiFi stopped");

            // Reinitialize WiFi
            WiFi.mode(WIFI_STA);
            WiFi.begin(ssid.c_str(), password.c_str());

            // Wait for connection
            while (WiFi.status() != WL_CONNECTED) {
                delay(1000);
                Serial.println("Connecting to WiFi...");
            }

            Serial.println("Connected to WiFi");
            Serial.print("IP Address: ");
            Serial.println(WiFi.localIP());
        } else {
            request->send(400, "text/plain", "Missing data");
        } });

  server.begin();
  Serial.println("HTTP server started");
}

void setup()
{
  Serial.begin(9600);

  // Initialize SPIFFS
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  else
  {
    Serial.println("SPIFFS mounted successfully");
  }

  // Reinitialize WiFi
  File file = SPIFFS.open("/config.txt", "r");
  if (!file)
  {
    Serial.println("Failed to open config file");
    init();
    return;
  }

  String ssid = file.readStringUntil('\n');
  String password = file.readStringUntil('\n');
  ssid.trim();
  password.trim();
  file.close();

  Serial.println("SSID: " + ssid);
  Serial.println("Password: " + password);

  if (ssid.length() == 0 || password.length() == 0)
  {
    Serial.println("Setup wizard started");
    init();
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
  // Monitor free heap memory
  // Serial.print("Free heap: ");
  // Serial.println(ESP.getFreeHeap());
  // delay(10); // Adjust delay as needed
}