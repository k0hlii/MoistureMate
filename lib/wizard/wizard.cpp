#include "SPIFFS.h"
#include <wizard.h>

AsyncWebServer server{80};

SetupWizard::SetupWizard()
{
    // Constructor code if needed
}

void SetupWizard::setup()
{

    WiFi.softAP(ssid, password);
    // preferences.begin("data", false);

    if (!preferences.begin("data", false))
    {
        Serial.println("Failed to initialize NVS");
        return;
    }

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
    server.on("/submit", HTTP_POST, [this](AsyncWebServerRequest *request)
    {
        String ssid, password, url, topic, name, ct;
        if (request->hasParam("ssid", true) && request->hasParam("password", true) &&
            request->hasParam("url", true) && request->hasParam("topic", true) &&
            request->hasParam("name", true) && request->hasParam("ct", true))
        {

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

            // Send a redirect response
            request->send(200, "text/html", "<html><body><script>window.location.href = '/complete';</script></body></html>");

            preferences.putString("ssid", ssid.c_str());
            preferences.putString("password", password.c_str());
            preferences.putString("url", url.c_str());
            preferences.putString("topic", topic.c_str());
            preferences.putString("name", name.c_str());
            preferences.putString("ct", ct.c_str());

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
        } 
        else
        {
            request->send(400, "text/plain", "Missing data");
        } });

    server.begin();
    Serial.println("HTTP server started");
}