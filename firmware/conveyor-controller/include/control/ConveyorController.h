#pragma once

class ConveyorMotor;

namespace ConveyorController
{
    void init(ConveyorMotor& motor);
    void update();
}