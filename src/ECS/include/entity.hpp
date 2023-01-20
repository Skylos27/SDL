#pragma once

#include <algorithm>
#include <array>
#include <queue>

#include "types.hpp"

namespace ECS {
    class EntityManager
    {
        public:
            explicit EntityManager();
            ~EntityManager() = default;

            Entity create();
            void destroyEntity(const Entity entity);

            void setSignature(Entity entity, Signature signature);
            Signature getSignature(Entity entity);
        private:
            std::queue<Entity> _availableEntities{};
            std::array<Signature, MAX_ENTITIES> _signatures{};
            uint32_t _entitiesCount{};
    };
};