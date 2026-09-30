#pragma once

#include <iostream>

class IManager
{
public:
    
    virtual bool DestroyObject(const uint32_t id) = 0;

    virtual ~IManager() = default;
};
