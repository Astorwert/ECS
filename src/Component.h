#pragma once

#include "Object.h"

using EntityId = uint32_t;
using ComponentId = std::uint32_t;

class Component : public Object
{
    
protected:
    
    EntityId ownerEntityId;

public:
    Component(const ComponentId newId, const EntityId entityId) : Object(newId), ownerEntityId(entityId)
    {

    }
};

