#include "include/entity.hpp"

ECS::EntityManager::EntityManager()
{
    for (auto i = 0; i < MAX_ENTITIES; i++)
        _availableEntities.push(i);
}

ECS::Entity ECS::EntityManager::create()
{
    auto entity = _availableEntities.front();
    _availableEntities.pop();
    _entitiesCount += 1;

    return entity;
}

void ECS::EntityManager::destroyEntity(const Entity entity)
{
    _signatures[entity].reset();
    _availableEntities.push(entity);
    _entitiesCount -= 1;
}

void ECS::EntityManager::setSignature(Entity entity, Signature signature)
{
    _signatures[entity] = signature;
}

ECS::Signature ECS::EntityManager::getSignature(Entity entity)
{
    return _signatures[entity];
}
