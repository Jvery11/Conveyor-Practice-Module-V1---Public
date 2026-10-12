
#pragma once

#include <WiFi.h>

namespace WiFiConnection
{
    bool init();
    void update();

    bool isConnected();
    IPAddress getIPAddress();
}
