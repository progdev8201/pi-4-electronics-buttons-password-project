#pragma once
#include "InputCallback.hpp"
#include <thread>

class InputPin
{
public:
    InputPin(const size_t pinNumber);
    int checkSignal();

private:
    int debouncedRead();

private:
    const size_t _pinNumber;
    InputCallback _inputCallback;
    const size_t DEBOUNCED_DELAY{10};
};

