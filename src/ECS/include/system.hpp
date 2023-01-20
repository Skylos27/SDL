#pragma once

#include "types.hpp"

#include <memory>
#include <set>
#include <unordered_map>

namespace ECS {

    class System
    {
        public:
            std::set<Entity> Entities;
    };

    class SystemManager
    {
    public:
        template<typename T>
        std::shared_ptr<T> RegisterSystem()
        {
            const char* typeName = typeid(T).name();

            auto system = std::make_shared<T>();
            _systems.insert({typeName, system});
            return system;
        }

        template<typename T>
        void SetSignature(Signature signature)
        {
            const char* typeName = typeid(T).name();

            _signatures.insert({typeName, signature});
        }

        void EntityDestroyed(Entity entity)
        {
            for (auto const& pair : _systems)
            {
                auto const& system = pair.second;

                system->Entities.erase(entity);
            }
        }

        void EntitySignatureChanged(Entity entity, Signature entitySignature)
        {
            // Notify each system that an entity's signature changed
            for (auto const& pair : _systems)
            {
                auto const& type = pair.first;
                auto const& system = pair.second;
                auto const& systemSignature = _signatures[type];

                if ((entitySignature & systemSignature) == systemSignature)
                {
                    system->Entities.insert(entity);
                } else
                {
                    system->Entities.erase(entity);
                }
            }
        }

    private:
        // Map from system type string pointer to a signature
        std::unordered_map<const char*, Signature> _signatures{};

        // Map from system type string pointer to a system pointer
        std::unordered_map<const char*, std::shared_ptr<System>> _systems{};
    };
}