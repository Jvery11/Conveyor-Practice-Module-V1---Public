#include "devices/ConveyorMotor.h"
#include <MKSServoCAN.h>

// Wrapper functions over currennt library ones

// tunable parameters for the conveyor motor. Adjust later
static constexpr uint8_t DEFAULT_ACCEL = 10;

// Flip if needed later based on how the motor is mounted on the conveyor.
static constexpr bool FORWARD_CCW = false;

ConveyorMotor::ConveyorMotor(uint32_t canId)
    : _canId(canId)
{
}

void ConveyorMotor::init()
{
    // Allow the motor to send CAN responses/status messages.
    MKSServoCAN::setCanResponse(_canId, true, true);

    MKSServoCAN::enableMotor(_canId, true);

    MKSServoCAN::pollResponses();
}

void ConveyorMotor::enable()
{
    MKSServoCAN::enableMotor(_canId, true);
}

void ConveyorMotor::disable()
{
    MKSServoCAN::enableMotor(_canId, false);
}

void ConveyorMotor::stop()
{
    MKSServoCAN::speedModeStop(_canId);
}

void ConveyorMotor::forward(uint16_t speed)
{
    MKSServoCAN::speedMode(
        _canId,
        speed,
        DEFAULT_ACCEL,
        FORWARD_CCW
    );
}

void ConveyorMotor::reverse(uint16_t speed)
{
    MKSServoCAN::speedMode(
        _canId,
        speed,
        DEFAULT_ACCEL,
        !FORWARD_CCW
    );
}

void ConveyorMotor::moveForward(uint32_t pulses, uint16_t speed)
{
    MKSServoCAN::posRelative(
        _canId,
        pulses,
        speed,
        DEFAULT_ACCEL,
        FORWARD_CCW
    );
}

void ConveyorMotor::moveBackward(uint32_t pulses, uint16_t speed)
{
    MKSServoCAN::posRelative(
        _canId,
        pulses,
        speed,
        DEFAULT_ACCEL,
        !FORWARD_CCW
    );
}