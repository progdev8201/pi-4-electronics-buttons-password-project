#include "InputCallback.hpp"

// Constructors

InputCallback::InputCallback(): _signalState{false},_released{true}
{

}

// Services

void InputCallback::operator()(const bool signalState)
{
    _signalState = signalState;
}

void InputCallback::setReleased(const bool released)
{
    _released = released;
}

bool InputCallback::released()
{
    return _released;
}

bool InputCallback::signalState()
{
    return _signalState;
}