#pragma once

#include "Arduino.h"

namespace ModbusServer
{
    void init();
    void update();

    bool getConveyorRunCommand();
    bool isCommunicationHealthy();
    uint16_t getConveyorSpeedCommand();
}