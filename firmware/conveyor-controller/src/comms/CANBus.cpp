#include "comms/CANBus.h"

#include <TwaiCan.h>
#include <MKSServoCAN.h>

#include "config.h"

namespace
{
    // CAN bus using the configured ESP32 pins
    TwaiCan bus(CAN_TX_PIN, CAN_RX_PIN);
}

namespace CANBus
{
    void init()
    {
        Serial.println("Starting CAN...");

        if (!MKSServoCAN::begin(&bus))
        {
            Serial.println("CAN initialization failed.");

            // CAN is required for conveyor operation,
            // so stop here if initialization fails.
            while (true)
            {
                delay(1000);
            }
        }

        Serial.println("CAN initialized.");
    }

    void update()
    {
        MKSServoCAN::pollResponses();
    }

    void wait(uint32_t durationMs)
    {
        uint32_t start = millis();

        while (millis() - start < durationMs)
        {
            update();
            delay(10);
        }
    }
}