#include "comms/ModbusServer.h"

#include <Arduino.h>
#include <ModbusIP_ESP8266.h>

namespace
{
    ModbusIP modbus;

    constexpr uint16_t CONVEYOR_RUN_COIL = 0;
    constexpr uint16_t ESP32_ACTIVE_DI = 0;

    constexpr uint16_t CONVEYOR_SPEED_HREG = 0;

    constexpr uint32_t COMMAND_WATCHDOG_TIMEOUT_MS = 1500;

    uint32_t lastCommandWriteMs = 0;

    bool commandReceived = false;
    bool communicationHealthy = false;

    // Called whenever the PLC writes to the conveyor run coil.
    uint16_t onConveyorRunWrite(TRegister *reg, uint16_t value)
    {
        lastCommandWriteMs = millis();
        commandReceived = true;

        if (!communicationHealthy)
        {
            communicationHealthy = true;
            Serial.println("PLC command communication established.");
        }

        return value;
    }

    void forceConveyorCommandOff()
    {
        // This is a local safe-state action, not a command received
        // from the PLC, so don't let it refresh the watchdog callback.
        modbus.cbDisable();
        modbus.Coil(CONVEYOR_RUN_COIL, false);
        modbus.cbEnable();
    }
}

namespace ModbusServer
{
    void init()
    {
        Serial.println("Starting Modbus TCP server...");

        modbus.server();

        modbus.addCoil(CONVEYOR_RUN_COIL, false);

        modbus.addIsts(ESP32_ACTIVE_DI, true);

        modbus.addHreg(CONVEYOR_SPEED_HREG, 0, 1);

        // Refresh watchdog whenever the conveyor run coil
        // receives a write from the PLC.
        modbus.onSetCoil(
            CONVEYOR_RUN_COIL,
            onConveyorRunWrite);

        // Safe startup state.
        commandReceived = false;
        communicationHealthy = false;
        forceConveyorCommandOff();

        Serial.println("Modbus TCP server started.");
    }

    void update()
    {
        // Service incoming Modbus requests.
        modbus.task();

        // ESP32 itself is operational.
        modbus.Ists(ESP32_ACTIVE_DI, true);

        if (commandReceived &&
            communicationHealthy &&
            (millis() - lastCommandWriteMs > COMMAND_WATCHDOG_TIMEOUT_MS))
        {
            communicationHealthy = false;

            // Force the conveyor command to its safe state.
            forceConveyorCommandOff();

            Serial.println(
                "PLC communication timeout - conveyor command forced OFF.");
        }
    }

    bool getConveyorRunCommand()
    {
        return communicationHealthy &&
               modbus.Coil(CONVEYOR_RUN_COIL);
    }

    bool isCommunicationHealthy()
    {
        return communicationHealthy;
    }

    uint16_t getConveyorSpeedCommand()
    {
        return modbus.Hreg(CONVEYOR_SPEED_HREG);
    }
}