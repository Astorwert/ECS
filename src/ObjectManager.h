#pragma once

#include <vector>
#include "IManager.h"

/**
 * @class ObjectManager
 * @brief Manages objects of a specific type.
 *
 * The ObjectManager class is responsible for creating, retrieving, and destroying objects
 * of a specific type. It provides efficient object management by using sparse storage and
 * recycling of indices.
 *
 * @tparam T The type of objects managed by the ObjectManager.
 */
template <class T>
class ObjectManager : public IManager
{
public:

    /**
     * Creates a new object with optional constructor arguments.
     * @tparam TArgs The types of constructor arguments.
     * @param args The arguments to be forwarded to the object's constructor.
     * @return The ID of the newly created object.
     */
    template<typename... TArgs>
    uint32_t CreateObject(TArgs&&... args)
    {
        uint32_t id;
        uint32_t lastIndex = static_cast<uint32_t>(sparseObjects.size());
        
        if (!freeIndices.empty())
        {
            id = freeIndices.back();
            freeIndices.pop_back();
            objectIdToSparseIndex[id] = lastIndex;
        }
        else
        {
            id = lastIndex;
            objectIdToSparseIndex.push_back(id);
        }

        sparseObjects.emplace_back(id, std::forward<TArgs>(args)...);
		
        return id;
    }

    /**
     * Retrieves a pointer to the object with the specified ID.
     * @param id The ID of the object to retrieve.
     * @return A pointer to the object, or nullptr if the object does not exist.
     */
    T* GetObject(const uint32_t id)
    {
        if (id >= objectIdToSparseIndex.size()) return nullptr;
        
        uint32_t index = objectIdToSparseIndex[id];
        return &sparseObjects[index];
    }

    /**
     * Retrieves a reference to the vector of objects.
     * @return A reference to the vector of objects.
     */
    std::vector<T>& getObjects()
    {
        return sparseObjects;
    }

    /**
     * Destroys the object with the specified ID.
     * @param id The ID of the object to destroy.
     * @return True if the object was successfully destroyed, false otherwise.
     */
    bool DestroyObject(const uint32_t id) override
    {
        if (id >= objectIdToSparseIndex.size()) return false;
        
        uint32_t objectIndex = objectIdToSparseIndex[id];
        
        if (objectIndex != sparseObjects.size() - 1)
        {
            T& lastObject = sparseObjects.back();
            uint32_t lastObjectId = lastObject.GetId();
            objectIdToSparseIndex[lastObjectId] = objectIndex;
            sparseObjects[objectIndex] = std::move(lastObject);
        }

        sparseObjects.pop_back();
        freeIndices.push_back(id);

        return true;
    }
    
private:

    std::vector<uint32_t> freeIndices; ///< The list of available indices for recycling.
    std::vector<uint32_t> objectIdToSparseIndex; ///< Maps object IDs to their indices in the sparseObjects vector.
    std::vector<T> sparseObjects; ///< The vector of objects with sparse storage.
};
