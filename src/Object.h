#pragma once
#include <iostream>

/**
 * @class Object
 * @brief Base class for objects in the scene.
 *
 * The Object class serves as the base class for objects in the scene.
 * It provides a unique identifier (ID) for each object.
 */
class Object
{
protected:
    
    uint32_t id; ///< The unique identifier (ID) of the object.
    
public:

    /**
     * Constructs an object with the specified ID.
     * @param newId The ID of the object.
     */
    Object(const uint32_t newId) : id(newId) {}

    /**
     * Retrieves the ID of the object.
     * @return The ID of the object.
     */
    uint32_t GetId() const
    {
        return id;
    }
};
