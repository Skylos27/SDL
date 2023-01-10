#include "./AI.hpp"

#include <iostream>

extern ECS::Coordinator coordinator;

void ECS::Systems::AISystem::init(int w, int h)
{
    _windowSize.x = w;
    _windowSize.y = h;
    
    _start = _clock.now();
}

void ECS::Systems::AISystem::update()
{
    auto now = _clock.now();
    auto interval = now - _start;

    if (std::chrono::duration_cast<std::chrono::milliseconds>(interval).count() < 5.0f) return;

    _start = _clock.now();

    for (auto &entity : Entities) {


        const auto &type = coordinator.GetComponent<ECS::Components::CharacterType>(entity);
        const auto &info = coordinator.GetComponent<ECS::Components::CharacterInfo>(entity);
        const auto &aiType = coordinator.GetComponent<ECS::Components::AITypeComponent>(entity);
        auto &position = coordinator.GetComponent<ECS::Components::Position>(entity);
        auto &direction = coordinator.GetComponent<ECS::Components::DirectionComponent>(entity);

        position.pos.x += direction.vec.x;
        position.pos.y += direction.vec.y;
    }
}