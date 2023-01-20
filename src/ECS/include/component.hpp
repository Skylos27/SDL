#pragma once

#include <unordered_map>
#include <memory>

#include "types.hpp"

namespace ECS {

    class IComponentArray
    {
        public:
            virtual ~IComponentArray() = default;
            virtual void entityDestroyed(Entity entity) = 0;
    };


    template<typename T>
    class ComponentArray : public IComponentArray
    {
    public:
        void insertData(Entity entity, T component)
        {
            size_t newIndex = _size;
            _entityToIndex[entity] = newIndex;
            _indexToEntity[newIndex] = entity;
            _componentArray[newIndex] = component;
            _size +=1;
        }

        void removeData(Entity entity)
        {
            size_t indexOfRemovedEntity = _entityToIndex[entity];
            size_t indexOfLastElement = _size - 1;
            _componentArray[indexOfRemovedEntity] = _componentArray[indexOfLastElement];

            Entity entityOfLastElement = _indexToEntity[indexOfLastElement];
            _entityToIndex[entityOfLastElement] = indexOfRemovedEntity;
            _indexToEntity[indexOfRemovedEntity] = entityOfLastElement;

            _entityToIndex.erase(entity);
            _indexToEntity.erase(indexOfLastElement);

            _size -= 1;
        }

        T& getData(Entity entity)
        {
            return _componentArray[_entityToIndex[entity]];
        }

        void entityDestroyed(Entity entity) override
        {
            if (_entityToIndex.find(entity) != _entityToIndex.end())
            {
                removeData(entity);
            }
        }

    private:
        std::array<T, MAX_ENTITIES> _componentArray;
        std::unordered_map<Entity, size_t> _entityToIndex;
        std::unordered_map<size_t, Entity> _indexToEntity;

        size_t _size;
    };

    class ComponentManager
    {
    public:
        template<typename T>
        void RegisterComponent()
        {
            const char* typeName = typeid(T).name();

            _componentTypes.insert({typeName, _nextComponent});
            _componentsArray.insert({typeName, std::make_shared<ComponentArray<T>>()});

            _nextComponent += 1;
        }

        template<typename T>
        ComponentType GetComponentType()
        {
            const char* typeName = typeid(T).name();

            return _componentTypes[typeName];
        }

        template<typename T>
        void AddComponent(Entity entity, T component)
        {
            GetComponentArray<T>()->insertData(entity, component);
        }

        template<typename T>
        void RemoveComponent(Entity entity)
        {
            GetComponentArray<T>()->removeData(entity);
        }

        template<typename T>
        T& GetComponent(Entity entity)
        {
            return GetComponentArray<T>()->getData(entity);
        }

        void entityDestroyed(Entity entity)
        {
            for (auto const& pair : _componentsArray)
            {
                auto const& component = pair.second;

                component->entityDestroyed(entity);
            }
        }

    private:
        std::unordered_map<const char*, ComponentType> _componentTypes{};
        std::unordered_map<const char*, std::shared_ptr<IComponentArray>> _componentsArray{};
        ComponentType _nextComponent{};

        template<typename T>
        std::shared_ptr<ComponentArray<T>> GetComponentArray()
        {
            const char* typeName = typeid(T).name();

            return std::static_pointer_cast<ComponentArray<T>>(_componentsArray[typeName]);
        }
    };

}