#pragma once
#include <iostream>
#include <mutex>
#include <thread>

class OutputPin
{
public:
    OutputPin(const size_t pinNumber);
    ~OutputPin();
    void startBlink(const size_t onDelay,const size_t offDelay);
    void stopBlink();
    
    void write(const size_t value);
    std::thread& thread();

private:

private:
    const size_t _pinNumber;
    const size_t _blinkingSeconds;
    bool _stop{false};
    std::mutex _mut;
    std::thread _thread;
};