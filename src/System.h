#pragma once

class Scene;

class System
{
public:
    virtual void Update(Scene* scene, float deltaTime) = 0;
    
    virtual ~System() = default;
};

