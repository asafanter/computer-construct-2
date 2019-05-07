#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>
using string = std::string;

class CacheSimulatorExceptions : public std::exception
{
public:
    CacheSimulatorExceptions() :
        _msg("CacheSimulatorException") {}

    const char* what() const noexcept
    {
        return _msg.c_str();
    }

protected:
    string _msg;
};

class WayInvalidSetNumber : public CacheSimulatorExceptions
{
public:
    WayInvalidSetNumber(const unsigned int &set) :
        _msg("WayInvalidSetNumber: set number " + std::to_string(set) +  " is illegal") {}

    const char* what() const noexcept
    {
        return _msg.c_str();
    }

protected:
    string _msg;
};

#endif // EXCEPTIONS_H
