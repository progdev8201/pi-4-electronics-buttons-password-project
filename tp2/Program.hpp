#pragma once
#include <iostream>
#include "InputPin.hpp"
#include "OutputPin.hpp"
#include <wiringPi.h>
#include <chrono>

class Program
{
public:
    Program(std::string password);
    void run();
    bool isPasswordValid(const std::string password);
    void setPassword(const std::string password);
    std::string password();
    size_t passwordFoundCount();
    size_t passwordFailedCount();
    void resetStats();

private:
    void blinkEveryone();
    void turnOffEveryone();
    void isGameOver();
    void handleClick(const char button);
    void lightAnswerPin();

private:
    std::string _password;
    size_t _passwordFoundCount{0};
    size_t _passwordFailedCount{0};
    size_t _counter{0};
    InputPin _buttonA{27};
    InputPin _buttonB{28};
    InputPin _buttonC{29};

    OutputPin _ledPinA{21};
    OutputPin _ledPinB{22};
    OutputPin _ledPinC{23};
};