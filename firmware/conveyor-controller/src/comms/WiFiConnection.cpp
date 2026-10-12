#include "comms/WiFiConnection.h"

#include <Arduino.h>
#include "secrets.h"

namespace
{
    constexpr unsigned long CONNECT_TIMEOUT_MS = 15000;
    constexpr unsigned long RECONNECT_INTERVAL_MS = 5000;

    unsigned long lastReconnectAttempt = 0;
}

namespace WiFiConnection
{
    bool init()
    {
        Serial.println("Connecting to WiFi...");

        WiFi.mode(WIFI_STA);

        
        WiFi.persistent(false);

   
        WiFi.setAutoReconnect(true);

        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

        unsigned long startTime = millis();

        while (WiFi.status() != WL_CONNECTED &&
               millis() - startTime < CONNECT_TIMEOUT_MS)
        {
            delay(250);
            Serial.print(".");
        }

        Serial.println();

        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.println("WiFi connected.");
            Serial.print("ESP32 IP address: ");
            Serial.println(WiFi.localIP());

            return true;
        }

        Serial.println("WiFi connection failed.");
        return false;
    }

    void update()
    {
        if (WiFi.status() == WL_CONNECTED)
            return;

        unsigned long currentTime = millis();

        if (currentTime - lastReconnectAttempt >= RECONNECT_INTERVAL_MS)
        {
            lastReconnectAttempt = currentTime;

            Serial.println("WiFi disconnected. Attempting reconnect...");
            WiFi.reconnect();
        }
    }

    bool isConnected()
    {
        return WiFi.status() == WL_CONNECTED;
    }

    IPAddress getIPAddress()
    {
        return WiFi.localIP();
    }
}


