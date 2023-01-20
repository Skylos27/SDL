#include "./RandomPath.hpp"

#include <iostream>

extern ECS::Coordinator coordinator;

void ECS::Systems::RandomPathSystem::init()
{
    mt = std::mt19937(rd());
    dist = std::uniform_int_distribution<std::uint16_t>(0, 32);
    _start = _clock.now();
}

void ECS::Systems::RandomPathSystem::update()
{
    auto now = _clock.now();
    auto interval = now - _start;

    if (std::chrono::duration_cast<std::chrono::milliseconds>(interval).count() < 250.0f) return;

    _start = _clock.now();

    for (auto &entity : Entities) {
        const uint16_t r = dist(mt);
        auto &direction = coordinator.GetComponent<ECS::Components::DirectionComponent>(entity);
        const auto &aitype = coordinator.GetComponent<ECS::Components::AITypeComponent>(entity);

        if (aitype.type != ECS::Components::AIType::PASSIVE) continue;
        if (direction.locked) continue;

        if (r == 1) {
            direction.vec.x = -1;
        } else if (r == 2) {
            direction.vec.x = 0;
        } else if (r == 3) {
            direction.vec.x = 1;
        } else if (r == 4) {
            direction.vec.y = -1;
        } else if (r == 5) {
            direction.vec.y = 0;
        } else if (r == 6) {
            direction.vec.y = 1;
        }
    }
}