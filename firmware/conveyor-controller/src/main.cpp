#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "config.h"

#include "comms/WiFiConnection.h"
#include "comms/CANBus.h"
#include "comms/ModbusServer.h"

#include "devices/ConveyorMotor.h"
#include "control/ConveyorController.h"

constexpr uint8_t ONBOARD_RGB_PIN = 16;

Adafruit_NeoPixel onboardRgb(
    1,
    ONBOARD_RGB_PIN,
    NEO_GRB + NEO_KHZ800
);

ConveyorMotor conveyor(CONVEYOR_MOTOR_ID);



void setup()
{
    Serial.begin(115200);
    delay(1000);

   
    // Turns off ultra bright LEDS
    onboardRgb.begin();
    onboardRgb.clear();
    onboardRgb.show();

    WiFiConnection::init();
    CANBus::init();

    conveyor.init();

    ModbusServer::init();
    ConveyorController::init(conveyor);
}


void loop()
{
    WiFiConnection::update();
    CANBus::update();
    ModbusServer::update();

    ConveyorController::update();
}