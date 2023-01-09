#include "include/component.hpp"

template <class T>
void ECS::ComponentArray::InserData(ECS::Entity entity, T component)
{
    assert(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end() && "Component added to same entity more than once.");

    // Put new entry at end and update the maps
    size_t newIndex = mSize;
    mEntityToIndexMap[entity] = newIndex;
    mIndexToEntityMap[newIndex] = entity;
    mComponentArray[newIndex] = component;
    ++mSize;
}

void RemoveData(Entity entity)
{
    assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Removing non-existent component.");

    // Copy element at end into deleted element's place to maintain density
    size_t indexOfRemovedEntity = mEntityToIndexMap[entity];
    size_t indexOfLastElement = mSize - 1;
    mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

    // Update map to point to moved spot
    Entity entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
    mEntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
    mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

    mEntityToIndexMap.erase(entity);
    mIndexToEntityMap.erase(indexOfLastElement);

    --mSize;
}

T& GetData(Entity entity)
{
    assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Retrieving non-existent component.");

    // Return a reference to the entity's component
    return mComponentArray[mEntityToIndexMap[entity]];
}

void EntityDestroyed(Entity entity) override
{
    if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end())
    {
        // Remove the entity's component if it existed
        RemoveData(entity);
    }
}