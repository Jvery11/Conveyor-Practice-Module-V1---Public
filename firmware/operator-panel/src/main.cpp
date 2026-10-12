#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "config.h"

#include "comms/WiFiConnection.h"
#include "comms/ModbusServer.h"

#include "devices/OperatorInputs.h"

constexpr uint8_t ONBOARD_RGB_PIN = 16;

Adafruit_NeoPixel onboardRgb(
    1,
    ONBOARD_RGB_PIN,
    NEO_GRB + NEO_KHZ800
);

OperatorInputs operatorPanel;

void setup()
{
    Serial.begin(115200);
    delay(1000);

   
    // Turns off ultra bright LEDS
    onboardRgb.begin();
    onboardRgb.clear();
    onboardRgb.show();

    operatorPanel.init();

    WiFiConnection::init();
    ModbusServer::init();


}

void loop() {

    WiFiConnection::update();
    operatorPanel.update();
    ModbusServer::update(operatorPanel);

}





