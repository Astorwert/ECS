#pragma once

#include "System.h"
#include <memory>
#include <vector>

/**
 * @class SystemManager
 * @brief Manages systems in the scene.
 *
 * The SystemManager class is responsible for managing systems in the scene.
 * It provides functionality to add systems and update all registered systems.
 */
class SystemManager
{
    std::vector<std::shared_ptr<System>> systems;
    
public:

    /**
     * Adds a system to the manager.
     * @tparam T The type of system to add.
     */
    template<typename T>
    void AddSystem()
    {
        systems.emplace_back(std::make_shared<T>());
    }

    /**
     * Updates all registered systems.
     * @param scene A pointer to the Scene object.
     * @param deltaTime The time elapsed since the last update.
     */
    void UpdateSystems(Scene* scene, float deltaTime) const
    {
        for (auto& it : systems)
        {
            it->Update(scene, deltaTime);
        }
    }
};
