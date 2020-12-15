#pragma once

class InputCallback
{
public:
    InputCallback();
    void operator()(const bool signalState);
    void setReleased(const bool released);
    bool signalState();
    bool released();

private:
    bool _signalState;
    bool _released;
};