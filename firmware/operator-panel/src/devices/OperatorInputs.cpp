#include "devices/OperatorInputs.h"
#include "config.h"

void OperatorInputs::init() {
    pinMode(START_BUTTON_PIN, INPUT_PULLUP);
    pinMode(STOP_BUTTON_PIN, INPUT_PULLUP);
    pinMode(RESET_BUTTON_PIN, INPUT_PULLUP);
    pinMode(JOG_STEP_BUTTON_PIN, INPUT_PULLUP);

    pinMode(MODE_A_PIN, INPUT_PULLUP);
    pinMode(MODE_B_PIN, INPUT_PULLUP);

    pinMode(SPEED_POT_PIN, INPUT);

    // Initialize debounce states to the actual current inputs
    bool startRaw = digitalRead(START_BUTTON_PIN) == LOW;
    bool stopRaw = digitalRead(STOP_BUTTON_PIN) == HIGH;   // NC stop
    bool resetRaw = digitalRead(RESET_BUTTON_PIN) == LOW;
    bool jogRaw = digitalRead(JOG_STEP_BUTTON_PIN) == LOW;

    bool modeARaw = digitalRead(MODE_A_PIN);
    bool modeBRaw = digitalRead(MODE_B_PIN);

    _startInput.stableState = startRaw;
    _startInput.lastRawState = startRaw;

    _stopInput.stableState = stopRaw;
    _stopInput.lastRawState = stopRaw;

    _resetInput.stableState = resetRaw;
    _resetInput.lastRawState = resetRaw;

    _jogStepInput.stableState = jogRaw;
    _jogStepInput.lastRawState = jogRaw;

    _modeAInput.stableState = modeARaw;
    _modeAInput.lastRawState = modeARaw;

    _modeBInput.stableState = modeBRaw;
    _modeBInput.lastRawState = modeBRaw;

    // Initialize pot filter at its real starting value
    _filteredSpeed = analogRead(SPEED_POT_PIN);
}

void OperatorInputs::updateDebouncedInput(
    DebouncedInput& input,
    bool rawState
) {
    unsigned long now = millis();

    if (rawState != input.lastRawState) {
        input.lastRawState = rawState;
        input.lastChangeTime = now;
    }

    if ((now - input.lastChangeTime) >= DEBOUNCE_MS) {
        input.stableState = rawState;
    }
}

void OperatorInputs::update() {

    // -------------------------
    // Buttons
    // -------------------------

    bool startRaw = digitalRead(START_BUTTON_PIN) == LOW;
    bool stopRaw = digitalRead(STOP_BUTTON_PIN) == HIGH; // NC
    bool resetRaw = digitalRead(RESET_BUTTON_PIN) == LOW;
    bool jogRaw = digitalRead(JOG_STEP_BUTTON_PIN) == LOW;

    updateDebouncedInput(_startInput, startRaw);
    updateDebouncedInput(_stopInput, stopRaw);
    updateDebouncedInput(_resetInput, resetRaw);
    updateDebouncedInput(_jogStepInput, jogRaw);

    _startPressed = _startInput.stableState;
    _stopPressed = _stopInput.stableState;
    _resetPressed = _resetInput.stableState;
    _jogStepPressed = _jogStepInput.stableState;


    // -------------------------
    // Mode selector
    // -------------------------

    bool modeARaw = digitalRead(MODE_A_PIN);
    bool modeBRaw = digitalRead(MODE_B_PIN);

    updateDebouncedInput(_modeAInput, modeARaw);
    updateDebouncedInput(_modeBInput, modeBRaw);

    bool modeA = _modeAInput.stableState;
    bool modeB = _modeBInput.stableState;

    if (!modeA && modeB) {
        _mode = OperatingMode::MANUAL;
    }
    else if (modeA && modeB) {
        _mode = OperatingMode::AUTO;
    }
    else if (modeA && !modeB) {
        _mode = OperatingMode::STEP;
    }
    else {
        // Invalid selector state
        _mode = OperatingMode::MANUAL;
    }


    // -------------------------
    // Speed potentiometer
    // -------------------------

    uint16_t newReading = analogRead(SPEED_POT_PIN);

    // Exponential moving average
    _filteredSpeed +=
        POT_FILTER_ALPHA * (newReading - _filteredSpeed);

    _speedRaw = static_cast<uint16_t>(_filteredSpeed);

    // Normalize to 0–100%
    long normalized = map(
        _speedRaw,
        POT_MIN_RAW,
        POT_MAX_RAW,
        0,
        100
    );

    normalized = constrain(normalized, 0, 100);

    _speedPercent = static_cast<uint8_t>(normalized);
}


// -------------------------
// Getters
// -------------------------

bool OperatorInputs::startPressed() const {
    return _startPressed;
}

bool OperatorInputs::stopPressed() const {
    return _stopPressed;
}

bool OperatorInputs::resetPressed() const {
    return _resetPressed;
}

bool OperatorInputs::jogStepPressed() const {
    return _jogStepPressed;
}

OperatingMode OperatorInputs::getMode() const {
    return _mode;
}

uint16_t OperatorInputs::getSpeedRaw() const {
    return _speedRaw;
}

uint8_t OperatorInputs::getSpeedPercent() const {
    return _speedPercent;
}
