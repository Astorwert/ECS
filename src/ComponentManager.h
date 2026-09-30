#pragma once
#include "Component.h"
#include "ObjectManager.h"

template<typename T>
class ComponentManager : public ObjectManager<T>
{
};

