#include "./DogPath.hpp"

#include <iostream>

extern ECS::Coordinator coordinator;

void ECS::Systems::DogPathSystem::init(const Entity &player)
{
    _start = _clock.now();
    _player = player;
}

void ECS::Systems::DogPathSystem::update()
{
    auto now = _clock.now();
    auto interval = now - _start;

    if (std::chrono::duration_cast<std::chrono::milliseconds>(interval).count() < 5.0f) return;

    _start = _clock.now();
    
    auto &dog = *Entities.begin();

    auto &direction = coordinator.GetComponent<ECS::Components::DirectionComponent>(dog);
    auto &dogPosition = coordinator.GetComponent<ECS::Components::Position>(dog);
    const auto &type = coordinator.GetComponent<ECS::Components::CharacterType>(dog);
    auto &boundary = coordinator.GetComponent<ECS::Components::DogBoundary>(dog);
    // const auto &playerPosition = coordinator.GetComponent<ECS::Components::Position>(_player);
    const auto &playerDirection = coordinator.GetComponent<ECS::Components::DirectionComponent>(_player);

    dogPosition.pos.x += playerDirection.velocity * playerDirection.vec.x;
    dogPosition.pos.y += playerDirection.velocity * playerDirection.vec.y;

    dogPosition.pos.x += direction.velocity * direction.vec.x;
    dogPosition.pos.y += direction.velocity * direction.vec.y;

    boundary.current.x += direction.velocity * direction.vec.x;
    boundary.current.y += direction.velocity * direction.vec.y;
    if (boundary.current.x > 0 && std::abs(boundary.current.x) > boundary.boundary.x) {
        boundary.current.x = 0;
        direction.vec.x = 0;
        direction.vec.y = 1;
    }
    
    if (boundary.current.x < 0 && std::abs(boundary.current.x) > boundary.boundary.x) {
        boundary.current.x = 0;
        direction.vec.x = 0;
        direction.vec.y = -1;
    }

    if (boundary.current.y > 0 && std::abs(boundary.current.y) > boundary.boundary.y) {
        boundary.current.y = 0;
        direction.vec.y = 0;
        direction.vec.x = -1;
    }

    if (boundary.current.y < 0 && std::abs(boundary.current.y) > boundary.boundary.y) {
        boundary.current.y = 0;
        direction.vec.y = 0;
        direction.vec.x = 1;
    }

    
    // boundary.current 
}