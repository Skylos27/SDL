#include "./DangerPath.hpp"

#include <iostream>

extern ECS::Coordinator coordinator;

void ECS::Systems::DangerPathSystem::init()
{
    _start = _clock.now();
}

std::optional<ECS::Components::Position> ECS::Systems::DangerPathSystem::isWolfNear(const ECS::Components::Position &posA)
{
    Entity wolf;
    bool found = false;

    for (const auto &x : Entities) {
        const auto &type = coordinator.GetComponent<ECS::Components::CharacterType>(x);
        
        if (type.type != ECS::Components::CharacterEnumType::WOLF) continue;
        const auto &posB = coordinator.GetComponent<ECS::Components::Position>(x);

        ECS::RectF r1{ posA.pos.x - 200, posA.pos.y - 200, 200, 400 };
        ECS::RectF r2{ posB.pos.x - 200, posB.pos.y - 200, 200, 400 };

        found = ECS::rectIntersects(r1, r2);
        if (found)
            wolf = x;
    }

    if (found)
        return coordinator.GetComponent<ECS::Components::Position>(wolf);
    return {};
}

std::optional<ECS::Components::Position> ECS::Systems::DangerPathSystem::isDogNear(const ECS::Components::Position &pos)
{
    Entity dog;
    bool found = false;

    for (const auto &x : Entities) {
        const auto &type = coordinator.GetComponent<ECS::Components::CharacterType>(x);
        
        if (type.type != ECS::Components::CharacterEnumType::DOG) continue;
        const auto &posB = coordinator.GetComponent<ECS::Components::Position>(x);

        ECS::RectF r1{ pos.pos.x - 150, pos.pos.y - 150, 150, 350 };
        ECS::RectF r2{ posB.pos.x - 150, posB.pos.y - 150, 150, 350 };

        found = ECS::rectIntersects(r1, r2);
        if (found)
            dog = x;
    }

    if (found) {
        return coordinator.GetComponent<ECS::Components::Position>(dog);
    }
    return {};
}

void ECS::Systems::DangerPathSystem::update()
{
    auto now = _clock.now();
    auto interval = now - _start;

    if (std::chrono::duration_cast<std::chrono::milliseconds>(interval).count() < 100.0f) return;

    _start = _clock.now();

    for (auto &entity : Entities) {
        auto &direction = coordinator.GetComponent<ECS::Components::DirectionComponent>(entity);
        const auto &position = coordinator.GetComponent<ECS::Components::Position>(entity);
        const auto &AItype = coordinator.GetComponent<ECS::Components::AITypeComponent>(entity);
        const auto &type = coordinator.GetComponent<ECS::Components::CharacterType>(entity);
        auto &info = coordinator.GetComponent<ECS::Components::CharacterInfo>(entity);

        if (type.type == ECS::Components::CharacterEnumType::SHEEP) {
            const std::optional<ECS::Components::Position> closestWolf = isWolfNear(position);
            
            if (closestWolf.has_value()) {
                
                if (closestWolf->pos.x > position.pos.x)
                    direction.vec.x = -1;
                else if (closestWolf->pos.x < position.pos.x)
                    direction.vec.x = 1;
                else
                    direction.vec.x = 0;

                if (closestWolf->pos.y > position.pos.y)
                    direction.vec.y = -1;
                else if (closestWolf->pos.y < position.pos.y)
                    direction.vec.y = 1;
                else
                    direction.vec.y = 0;

                if (std::chrono::duration_cast<std::chrono::seconds>(_clock.now() - info.lastSpeedBoost).count() > 5.0f) {
                    direction.velocity = 1.5f;
                    info.lastSpeedBoost = _clock.now();
                    direction.locked = true;
                }
            }

        } else if (type.type == ECS::Components::CharacterEnumType::WOLF) {
            const std::optional<ECS::Components::Position> dog = isDogNear(position);

            if (direction.locked == false && dog.has_value()) {
                if (dog->pos.x > position.pos.x)
                    direction.vec.x = -1;
                else if (dog->pos.x < position.pos.x)
                    direction.vec.x = 1;
                else
                    direction.vec.x = 0;

                if (dog->pos.y > position.pos.y)
                    direction.vec.y = -1;
                else if (dog->pos.y < position.pos.y)
                    direction.vec.y = 1;
                else
                    direction.vec.y = 0;
            }
        }
    }
}