#include "comms/ModbusServer.h"

#include <Arduino.h>
#include <ModbusIP_ESP8266.h>

namespace
{
    ModbusIP modbus;

    // Momentary
    constexpr uint16_t START_BUTTON_DI = 0;
    constexpr uint16_t STOP_BUTTON_DI = 1; 
    constexpr uint16_t RESET_BUTTON_DI = 2;
    constexpr uint16_t JOG_STEP_BUTTON_DI = 3;

    // 3 position switch
    constexpr uint16_t OPERATING_MODE_IREG = 0;

    // Speed potentiometer
    constexpr uint16_t POT_SPEED_IREG = 1;

}

namespace ModbusServer
{

    void init()
    {

        Serial.println("Starting Modbus Server");
        modbus.server();

        modbus.addIsts(0, false, 4);

        modbus.addIreg(0, 0, 2);

    }

    void update(const OperatorInputs &operatorPanel)
    {
        modbus.Ists(START_BUTTON_DI, operatorPanel.startPressed());
        modbus.Ists(STOP_BUTTON_DI, operatorPanel.stopPressed());
        modbus.Ists(RESET_BUTTON_DI, operatorPanel.resetPressed());
        modbus.Ists(JOG_STEP_BUTTON_DI, operatorPanel.jogStepPressed());

        modbus.Ireg(OPERATING_MODE_IREG, (uint16_t)operatorPanel.getMode());
        
        modbus.Ireg(POT_SPEED_IREG, (uint16_t)operatorPanel.getSpeedPercent());

        modbus.task();
    }
}
