#pragma once

#include <stdexcept>

class BCJsonException : public std::runtime_error
{
public:
    BCJsonException(const char* msg)
        : runtime_error(msg)
    {}
};
