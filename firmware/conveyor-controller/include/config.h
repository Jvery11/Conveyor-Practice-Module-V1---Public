#pragma once

#include <Arduino.h>

// CAN interface
constexpr gpio_num_t CAN_TX_PIN = GPIO_NUM_33; // yellow wire
constexpr gpio_num_t CAN_RX_PIN = GPIO_NUM_32; // green wire

// MKS Servo42D
constexpr uint32_t CONVEYOR_MOTOR_ID = 1;