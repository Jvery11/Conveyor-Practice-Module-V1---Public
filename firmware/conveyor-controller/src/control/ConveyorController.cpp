#include "Arduino.h"

#include "control/ConveyorController.h"

#include "devices/ConveyorMotor.h"
#include "comms/ModbusServer.h"

namespace
{
    ConveyorMotor *conveyor = nullptr;

    bool running = false;

    uint16_t lastSpeedCommandSent = 0;

    // Min/Max allowed speed values
    // For V1 these will just result in deadspots on the potentiometer
    // Will calibrate in future versions.
    constexpr uint16_t SPEED_MIN = 25;
    constexpr uint16_t SPEED_MAX = 90;
}

namespace ConveyorController
{

    void init(ConveyorMotor &motor)
    {
        conveyor = &motor;

        // Safe startup state
        conveyor->stop();
        running = false;
    }

    void update()
    {
        if (conveyor == nullptr)
            return;

        bool runCommand = ModbusServer::getConveyorRunCommand();

        uint16_t currentRequestedSpeed = ModbusServer::getConveyorSpeedCommand();

        if (currentRequestedSpeed < SPEED_MIN)
        {
            currentRequestedSpeed = SPEED_MIN;
        }
        else if (currentRequestedSpeed > SPEED_MAX)
        {
            currentRequestedSpeed = SPEED_MAX;
        }

        // Start only when command changes OFF -> ON
        if (runCommand && !running)
        {
            conveyor->forward(currentRequestedSpeed);
            running = true;

            lastSpeedCommandSent = currentRequestedSpeed;

            Serial.println("Conveyor started.");
        }
        else if (runCommand && running)
        {

            if (currentRequestedSpeed != lastSpeedCommandSent)
            {
                conveyor->forward(currentRequestedSpeed);
                lastSpeedCommandSent = currentRequestedSpeed;
            }
        }

        // Stop only when command changes ON -> OFF
        else if (!runCommand && running)
        {
            conveyor->stop();
            running = false;

            Serial.println("Conveyor stopped.");
        }
    }
}