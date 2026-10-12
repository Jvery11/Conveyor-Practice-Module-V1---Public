#pragma once

#include <Arduino.h>

// Position 1: A=0, B=1
// Center:     A=1, B=1
// Position 3: A=1, B=0

enum class OperatingMode
{
    MANUAL = 0,
    AUTO = 1,
    STEP = 2
};

class OperatorInputs
{
public:
    void init();
    void update();

    bool startPressed() const;
    bool stopPressed() const;
    bool resetPressed() const;
    bool jogStepPressed() const;

    OperatingMode getMode() const;

    uint16_t getSpeedRaw() const;
    uint8_t getSpeedPercent() const;

private:
    static constexpr unsigned long DEBOUNCE_MS = 30;

    // Pot filtering
    static constexpr float POT_FILTER_ALPHA = 0.15f;
    static constexpr uint16_t POT_MIN_RAW = 0;
    static constexpr uint16_t POT_MAX_RAW = 4095;

    struct DebouncedInput
    {
        bool stableState = false;
        bool lastRawState = false;
        unsigned long lastChangeTime = 0;
    };

    void updateDebouncedInput(DebouncedInput &input, bool rawState);

    DebouncedInput _startInput;
    DebouncedInput _stopInput;
    DebouncedInput _resetInput;
    DebouncedInput _jogStepInput;

    DebouncedInput _modeAInput;
    DebouncedInput _modeBInput;

    bool _startPressed = false;
    bool _stopPressed = false;
    bool _resetPressed = false;
    bool _jogStepPressed = false;

    OperatingMode _mode = OperatingMode::MANUAL;

    float _filteredSpeed = 0.0f;
    uint16_t _speedRaw = 0;
    uint8_t _speedPercent = 0;
};
