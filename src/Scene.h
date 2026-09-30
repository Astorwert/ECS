#pragma once

#include "ComponentManager.h"
#include "EntityManager.h"
#include "SystemManager.h"

/**
 * @class Scene
 * @brief Represents a scene in the application.
 *
 * The Scene class manages entities, components, and systems within a scene.
 * It provides functionality to create and destroy entities, register component types,
 * assign components to entities, retrieve components from entities, and update systems.
 */
class Scene
{
public:
    /**
     * Creates a new entity in the scene.
     * @return The ID of the newly created entity.
     */
    EntityId CreateEntity()
    {
        return entityManager.CreateObject();
    }

    /**
     * Destroys an entity in the scene.
     * @param entityId The ID of the entity to destroy.
     */
    void DestroyEntity(const EntityId entityId)
    {
        Entity* entity = entityManager.GetObject(entityId);

        auto& components = entity->GetAllComponents();
        
        for(const auto& componentPair: components)
        {
            const ComponentType componentType = componentPair.first;
            const ComponentId componentId = componentPair.second;
            componentManagerPool[componentType]->DestroyObject(componentId);
        }
        
        entityManager.DestroyObject(entityId);
    }

    /**
     * Registers a component type in the scene.
     * @tparam T The component type to register.
     */
    template<typename T>
    void RegisterComponent()
    {
        ComponentType componentType = typeid(T).name();
        componentManagerPool.emplace(componentType, std::make_shared<ComponentManager<T>>());
    }

    /**
     * @brief Assigns a component of a given type to an entity.
     * @tparam T The type of the component to assign.
     * @tparam TArgs The argument types for constructing the component.
     * @param entityId The ID of the entity to assign the component to.
     * @param args The arguments to forward to the component's constructor.
     * @return A pointer to the assigned component.
     */
    template<typename T, typename... TArgs>
    T* AssignEntityComponent(const EntityId entityId, TArgs&&... args)
    {
        auto componentManager = GetComponentManager<T>();
        ComponentId componentId = componentManager->CreateObject(entityId, std::forward<TArgs>(args)...);
        
        Entity* entity = entityManager.GetObject(entityId);
        entity->AddComponent<T>(componentId);
        
        return componentManager->GetObject(componentId);
    }

    /**
     * Retrieves a component from an entity.
     * @tparam T The component type to retrieve.
     * @param entityId The ID of the entity to retrieve the component from.
     * @return A pointer to the retrieved component, or nullptr if the component doesn't exist.
     */
    template<typename T>
    T* GetEntityComponent(const EntityId entityId)
    {
        Entity* entity = entityManager.GetObject(entityId);

        if (!entity->HasComponent<T>()) return nullptr;
        ComponentId componentId = entity->GetComponent<T>();
        
        return GetComponentManager<T>()->GetObject(componentId);
    }

    /**
     * Removes a component from an entity.
     * @tparam T The component type to remove.
     * @param entityId The ID of the entity to remove the component from.
     */
    template<typename T>
    void RemoveEntityComponent(const EntityId entityId)
    {
        Entity* entity = entityManager.GetObject(entityId);

        if (!entity->HasComponent<T>()) return;
        
        ComponentId componentId = entity->GetComponent<T>();
        entity->RemoveComponent<T>();
        
        GetComponentManager<T>()->DestroyObject(componentId);
    }

    /**
     * Retrieves the component manager for a given component type.
     * @tparam T The component type.
     * @return A shared pointer to the component manager.
     */
    template<typename T>
    std::shared_ptr<ComponentManager<T>> GetComponentManager()
    {
        ComponentType componentType = typeid(T).name();
        return std::static_pointer_cast<ComponentManager<T>>(componentManagerPool[componentType]);
    }

    /**
     * Retrieves a vector of components of a given type.
     * @tparam T The component type.
     * @return A reference to the vector of components.
     */
    template<typename T>
    std::vector<T>& GetComponents()
    {
        return GetComponentManager<T>()->getObjects();
    }

    /**
     * Registers a system in the scene.
     * @tparam T The system type to register.
     */
    template<typename T>
    void RegisterSystem()
    {
        return systemManager.AddSystem<T>();
    }

    /**
     * Updates the systems in the scene.
     * @param deltaTime The time since the last update.
     */
    void Update(float deltaTime)
    {
        systemManager.UpdateSystems(this, deltaTime);
    }

private:

    EntityManager entityManager; ///< The entity manager for managing entities.
    std::unordered_map<ComponentType, std::shared_ptr<IManager>> componentManagerPool; ///< The pool of component managers.
    SystemManager systemManager; ///< The system manager for managing systems.
};
