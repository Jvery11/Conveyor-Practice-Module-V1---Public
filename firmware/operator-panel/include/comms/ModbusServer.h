#pragma once

#include "devices/OperatorInputs.h"

namespace ModbusServer
{
    void init();
    void update(const OperatorInputs& operatorPanel);

}