#pragma once

#include <unordered_map>
#include "Object.h"

using EntityId = uint32_t;
using ComponentType = const char*;
using ComponentId = std::uint32_t;

class Scene;

/**
 * @class Entity
 * @brief Represents an entity in the scene.
 *
 * The Entity class represents an entity in the scene. It inherits from the Object class and provides
 * functionality to manage storage of components ID associated with the entity.
 */
class Entity : public Object
{
	std::unordered_map<ComponentType, ComponentId> typeComponentIdMap; ///< Map of component types to component IDs.

public:

	/**
	 * Constructs an entity with the specified ID.
	 * @param newId The ID of the entity.
	 */
	Entity(const EntityId newId) : Object(newId) {}


	/**
	 * Retrieves a reference to the map of all components associated with the entity.
	 * @return Reference to the map of component types to component IDs.
	 */
	std::unordered_map<ComponentType, ComponentId>& GetAllComponents()
	{
		return typeComponentIdMap;
	}

	/**
	 * Checks if the entity has a component of the specified type.
	 * @tparam T The type of the component.
	 * @return True if the entity has a component of the specified type, false otherwise.
	 */
	template<typename T>
	bool HasComponent()
	{
		return typeComponentIdMap.count(typeid(T).name()) > 0;
	}

	/**
	 * Retrieves the ID of the component of the specified type associated with the entity.
	 * @tparam T The type of the component.
	 * @return The ID of the component if it exists, otherwise an undefined value.
	 */
	template<typename T>
	ComponentId GetComponent()
	{
		return typeComponentIdMap[typeid(T).name()];
	}

	/**
	 * Adds a component to the entity.
	 * @tparam T The type of the component.
	 * @param componentId The ID of the component to be added.
	 */
	template<typename T>
	void AddComponent(ComponentId componentId)
	{
		typeComponentIdMap.emplace(typeid(T).name(), componentId);
	}

	/**
	 * Removes the component of the specified type from the entity.
	 * @tparam T The type of the component.
	 */
	template<typename T>
	void RemoveComponent()
	{
		typeComponentIdMap.erase(typeid(T).name());
	}
};

