#include "ConfigManager.h"

ConfigManager::ConfigManager()
{
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
}

bool ConfigManager::loadConfig()
{
    File file = SPIFFS.open("/config.txt", "r");
    if (!file)
    {
        Serial.println("Failed to open config file");
        return false;
    }

    while (file.available())
    {
        String line = file.readStringUntil('\n');
        line.trim();
        if (line.startsWith("SSID:"))
        {
            parseLine(line, "SSID:", ssid);
        }
        else if (line.startsWith("PW:"))
        {
            parseLine(line, "PW:", password);
        }
        else if (line.startsWith("URL:"))
        {
            parseLine(line, "URL:", url);
        }
        else if (line.startsWith("Topic:"))
        {
            parseLine(line, "Topic:", topic);
        }
        else if (line.startsWith("NAME:"))
        {
            parseLine(line, "NAME:", name);
        }
        else if (line.startsWith("CT:"))
        {
            parseLine(line, "CT:", ct);
        }
    }

    file.close();
    return true;
}

bool ConfigManager::saveConfig()
{
    File file = SPIFFS.open("/config.txt", "w");
    if (!file)
    {
        Serial.println("Failed to open config file for writing");
        return false;
    }

    file.println("SSID: " + ssid + ";");
    file.println("PW: " + password + ";");
    file.println("URL: " + url + ";");
    file.println("Topic: " + topic + ";");
    file.println("NAME: " + name + ";");
    file.println("CT: " + ct + ";");

    file.close();
    return true;
}

bool ConfigManager::parseLine(const String &line, const String &key, String &value)
{
    int startIndex = line.indexOf(key);
    if (startIndex == -1)
    {
        return false;
    }
    startIndex += key.length(); // Move past the key to the start of the value
    int endIndex = line.indexOf(';', startIndex);
    if (endIndex == -1)
    {
        return false;
    }
    value = line.substring(startIndex, endIndex); // Extract the substring
    value.trim();                                 // Trim whitespace in place
    return true;
}

// Getters
String ConfigManager::getSSID() const { return ssid; }
String ConfigManager::getPassword() const { return password; }
String ConfigManager::getURL() const { return url; }
String ConfigManager::getTopic() const { return topic; }
String ConfigManager::getName() const { return name; }
String ConfigManager::getCT() const { return ct; }

// Setters
void ConfigManager::setSSID(const String &ssid) { this->ssid = ssid; }
void ConfigManager::setPassword(const String &password) { this->password = password; }
void ConfigManager::setURL(const String &url) { this->url = url; }
void ConfigManager::setTopic(const String &topic) { this->topic = topic; }
void ConfigManager::setName(const String &name) { this->name = name; }
void ConfigManager::setCT(const String &ct) { this->ct = ct; }