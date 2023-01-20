#include "./GameState.hpp"

#include <iostream>

extern ECS::Coordinator coordinator;

void ECS::Systems::GameStateSystem::init()
{
    _start = _clock.now();
}

bool ECS::Systems::GameStateSystem::gameShouldEnd()
{
    auto now = _clock.now();
    auto interval = now - _start;

    if (std::chrono::duration_cast<std::chrono::milliseconds>(interval).count() < 100.0f) return false;
    

    _start = _clock.now();

    std::uint32_t sheeps = 0;
    std::uint32_t wolves = 0;

    for (const auto &entity : Entities) {
        const auto &type = coordinator.GetComponent<ECS::Components::CharacterType>(entity);

        if (type.type == ECS::Components::CharacterEnumType::SHEEP) sheeps += 1;
        if (type.type == ECS::Components::CharacterEnumType::WOLF) wolves += 1;
    }
    return wolves == 0 || sheeps == 0;
}