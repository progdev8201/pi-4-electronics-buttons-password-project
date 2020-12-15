#include "InputPin.hpp"
#include <wiringPi.h>
#include <iostream>

InputPin::InputPin(const size_t pinNumber) : _pinNumber{pinNumber}
{
    pinMode(_pinNumber, INPUT);
}

int InputPin::debouncedRead()
{
    bool isStable = false;
    int readVal;

    while (!isStable)
    {
        readVal = digitalRead(_pinNumber);
        delay(DEBOUNCED_DELAY);

        if (readVal == digitalRead(_pinNumber))
            isStable = true;
    }

    return readVal;
}

int InputPin::checkSignal()
{
    // make sure that user has released button since last click
    // if his finger is still on the button set signal state to false

    const bool clicked{debouncedRead() == HIGH};

    if (clicked && _inputCallback.released())
    {
        _inputCallback(true);
        _inputCallback.setReleased(false);
    }
    else if (clicked)
        _inputCallback(false);

    if (!clicked)
    {
        _inputCallback(false);
        _inputCallback.setReleased(true);
    }

    return _inputCallback.signalState();
}