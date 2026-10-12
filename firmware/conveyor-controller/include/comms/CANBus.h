#pragma once

#include <Arduino.h>

namespace CANBus
{
    void init();
    void update();
    void wait(uint32_t durationMs);
}