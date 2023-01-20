#include "./SpeedSystem.hpp"

#include <iostream>

extern ECS::Coordinator coordinator;

void ECS::Systems::SpeedSystem::init()
{
    _start = _clock.now();
}

void ECS::Systems::SpeedSystem::update()
{
    auto now = _clock.now();
    auto interval = now - _start;

    if (std::chrono::duration_cast<std::chrono::milliseconds>(interval).count() < 1.0f) return;

    _start = _clock.now();
    for (auto &entity : Entities) {
        auto &info = coordinator.GetComponent<ECS::Components::CharacterInfo>(entity);
        auto &direction = coordinator.GetComponent<ECS::Components::DirectionComponent>(entity);

        if (std::chrono::duration_cast<std::chrono::seconds>(_clock.now() - info.lastSpeedBoost).count() > 1.0f) {
            direction.velocity = direction.baseVelocity;
            // direction.locked = false;
        }
    }
}