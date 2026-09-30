// EntityComponentSystem.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <chrono>
#include "Scene.h"

class PositionComponent : public Component {
public:
    PositionComponent(const ComponentId newId, const EntityId entityId, float x, float y)
    : Component(newId, entityId), xPos(x), yPos(y) {}

    float xPos = 0;
    float yPos = 0;
};

class VelocityComponent : public Component {
public:
    VelocityComponent(const ComponentId newId, const EntityId entityId, float x, float y, float v)
    : Component(newId, entityId), xDir(x), yDir(y), speed(v) {}

    float xDir = 0;
    float yDir = 0;
    float speed = 0;
};

class RenderComponent : public Component {
public:
    RenderComponent(const ComponentId newId, const EntityId entityId, const char* text)
    : Component(newId, entityId), name(text){}

    const char* name;
};

class VelocitySystem : public System
{
    virtual void Update(Scene* scene, float deltaTime) override
    {
        std::vector<VelocityComponent> velocityPool = scene->GetComponents<VelocityComponent>();
        for (auto& velocity : velocityPool)
        {
            EntityId entityId = velocity.GetId();
            PositionComponent* position = scene->GetEntityComponent<PositionComponent>(entityId);
            if (position)
            {
                float distance = velocity.speed * deltaTime;
                position->xPos += velocity.xDir * distance;
                position->yPos += velocity.yDir * distance;
                std::cout << "Position changed: X = " << position->xPos << " Y = " << position->yPos<< std::endl;
            }
        }
    }
};

class RenderSystem : public System
{
    virtual void Update(Scene* scene, float deltaTime) override
    {
        std::vector<RenderComponent> renderPool = scene->GetComponents<RenderComponent>();
        for (auto& render : renderPool)
        {
            EntityId entityId = render.GetId();
            PositionComponent* position = scene->GetEntityComponent<PositionComponent>(entityId);
            if (position)
            {
                std::cout << "Rendered " << render.name << " in position: X = " << position->xPos << " Y = " << position->yPos<< std::endl;
            }
        }
    }
};

void setupScene(Scene& scene)
{
    scene.RegisterComponent<PositionComponent>();
    scene.RegisterComponent<VelocityComponent>();
    scene.RegisterComponent<RenderComponent>();

    scene.RegisterSystem<VelocitySystem>();
    scene.RegisterSystem<RenderSystem>();
}

void setupEntity(Scene& scene, EntityId hero, EntityId enemy)
{
    scene.AssignEntityComponent<PositionComponent>(hero, 0.f, 0.f);
    scene.AssignEntityComponent<VelocityComponent>(hero, 1.f, 1.f, 10.f);
    scene.AssignEntityComponent<RenderComponent>(hero, "Hero");

    scene.AssignEntityComponent<PositionComponent>(enemy, 10.f, 10.f);
    scene.AssignEntityComponent<VelocityComponent>(enemy, -1.f, -1.f, 20.f);
    scene.AssignEntityComponent<RenderComponent>(enemy, "Enemy");
}

void clearEntity(Scene& scene, EntityId hero, EntityId enemy)
{
    scene.RemoveEntityComponent<VelocityComponent>(hero);
    scene.RemoveEntityComponent<RenderComponent>(hero);
    scene.RemoveEntityComponent<PositionComponent>(hero);
    scene.DestroyEntity(hero);
    
    scene.DestroyEntity(enemy);
}


int main()
{
    Scene scene;
    EntityId hero = scene.CreateEntity();
    EntityId enemy = scene.CreateEntity();

    setupScene(scene);
    setupEntity(scene, hero, enemy);

    float timer = 3.f;
    float deltaTime = 0.0f;

    while (timer > 0)
    {
        auto startTime = std::chrono::high_resolution_clock::now();

        scene.Update(deltaTime);

        auto stopTime = std::chrono::high_resolution_clock::now();

        deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();
        timer -= deltaTime;
    }

    clearEntity(scene, hero, enemy);
    
    return 0;
}