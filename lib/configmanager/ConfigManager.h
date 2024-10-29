#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <Arduino.h>
#include "SPIFFS.h"

class ConfigManager {
public:
    ConfigManager();
    bool loadConfig();
    bool saveConfig();

    // Getters
    String getSSID() const;
    String getPassword() const;
    String getURL() const;
    String getTopic() const;
    String getName() const;
    String getCT() const;

    // Setters
    void setSSID(const String& ssid);
    void setPassword(const String& password);
    void setURL(const String& url);
    void setTopic(const String& topic);
    void setName(const String& name);
    void setCT(const String& ct);

private:
    String ssid;
    String password;
    String url;
    String topic;
    String name;
    String ct;

    bool parseLine(const String& line, const String& key, String& value);
};

#endif // CONFIG_MANAGER_H