#include "Program.hpp"

// Constructors
Program::Program(std::string password) : _password{password}
{
}

// Services

void Program::run()
{
    while (1)
    {
        if (_buttonA.checkSignal())
            handleClick('C');

        if (_buttonB.checkSignal())
            handleClick('B');

        if (_buttonC.checkSignal())
            handleClick('A');
    }
}

void Program::handleClick(const char button)
{
    if (_password.at(_counter) == button)
    {
        lightAnswerPin();

        isGameOver();
    }
    else
        turnOffEveryone();
}


void Program::lightAnswerPin()
{
    switch (++_counter)
    {
    case 1:
        _ledPinA.write(HIGH);
        break;
    case 2:
        _ledPinB.write(HIGH);
        break;
    case 3:
        _ledPinC.write(HIGH);
        break;
    default:
        break;
    }
}

void Program::isGameOver()
{
    if (_counter == _password.length())
    {
        blinkEveryone();

        //SLEEP THEN RESTART GAME
        _counter = 0;
    }
}

void Program::blinkEveryone()
{
    _passwordFoundCount++;

    _ledPinA.startBlink(1, 1);
    _ledPinB.startBlink(1, 1);
    _ledPinC.startBlink(1, 1);

    std::this_thread::sleep_for(std::chrono::seconds{5});

    _ledPinA.stopBlink();
    _ledPinB.stopBlink();
    _ledPinC.stopBlink();
}

void Program::turnOffEveryone()
{
    _passwordFailedCount++;

    _ledPinA.write(LOW);
    _ledPinB.write(LOW);
    _ledPinC.write(LOW);

    _counter = 0;
}

void Program::setPassword(const std::string password)
{
    _password = password;
}

bool Program::isPasswordValid(const std::string password)
{
    if (password.length() != 3)
    {
        std::cout << "password should contain three characters" << std::endl;
        return false;
    }

    for (size_t i = 0; i < password.length(); i++)
    {
        if (password.at(i) != 'A' && password.at(i) != 'B' && password.at(i) != 'C')
        {
            std::cout << "password should only contain A B or C" << std::endl;
            return false;
        }
    }

    return true;
}

size_t Program::passwordFoundCount()
{
    return _passwordFoundCount;
}
size_t Program::passwordFailedCount()
{
    return _passwordFailedCount;
}

void Program::resetStats()
{
    _passwordFoundCount = 0;
    _passwordFailedCount = 0;
}

std::string Program::password()
{
    return _password;
}