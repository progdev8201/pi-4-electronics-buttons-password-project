#include "OutputPin.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <wiringPi.h>

// Constructors

OutputPin::OutputPin(const size_t pinNumber) : _pinNumber{pinNumber}, _blinkingSeconds{5}
{
    pinMode(_pinNumber, OUTPUT);
    digitalWrite(_pinNumber, LOW);
}

OutputPin::~OutputPin()
{
    if (_thread.joinable())_thread.join();
}

// Services

void OutputPin::startBlink(const size_t onDelay, const size_t offDelay)
{
     if(_thread.joinable())_thread.join();

    _thread = std::thread{[this, onDelay, offDelay]() {
        while (1)
        {
            {
                std::lock_guard<std::mutex> lg{_mut};

                if (_stop)
                {
                    _stop = false;
                    break;
                }
            }

            write(HIGH);
            std::this_thread::sleep_for(std::chrono::seconds{onDelay});

            write(LOW);
            std::this_thread::sleep_for(std::chrono::seconds{offDelay});
        }
    }};
}

void OutputPin::stopBlink()
{
    {
        std::lock_guard<std::mutex> lg{_mut};

        _stop = true;
    }
}

void OutputPin::write(const size_t value)
{
    digitalWrite(_pinNumber, value);
}

std::thread &OutputPin::thread()
{
    return _thread;
}