#pragma once

#include <Arduino.h>


// defines basic motor controls for the conveyor belt motor
class ConveyorMotor
{
public:
    explicit ConveyorMotor(uint32_t canId);

    void init();

    void enable();
    void disable();
    void stop();

    void forward(uint16_t speed);
    void reverse(uint16_t speed);

    void moveForward(uint32_t pulses, uint16_t speed);
    void moveBackward(uint32_t pulses, uint16_t speed);


private:
    uint32_t _canId;
};