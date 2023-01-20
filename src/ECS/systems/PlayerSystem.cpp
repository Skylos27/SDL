#include "./PlayerSystem.hpp"

#include <iostream>

extern ECS::Coordinator coordinator;

void ECS::Systems::PlayerSystem::init()
{
    _start = _clock.now();
}

void ECS::Systems::PlayerSystem::handleKeyUpEvent(const ECS::Components::PlayerInputType &input)
{
    const auto playerEntity = *Entities.cbegin();
    auto &direction = coordinator.GetComponent<ECS::Components::DirectionComponent>(playerEntity);

    if (input == ECS::Components::PlayerInputType::PLAYER_UP) {
        direction.vec.y =-1;
    } else if (input == ECS::Components::PlayerInputType::PLAYER_DOWN) {
        direction.vec.y = 1;
    } else if (input == ECS::Components::PlayerInputType::PLAYER_LEFT) {
        direction.vec.x = -1;
    } else if (input == ECS::Components::PlayerInputType::PLAYER_RIGHT) {
        direction.vec.x = 1;
    }
}

void ECS::Systems::PlayerSystem::handleKeyDownEvent(const ECS::Components::PlayerInputType &input)
{
    const auto playerEntity = *Entities.cbegin();
    auto &direction = coordinator.GetComponent<ECS::Components::DirectionComponent>(playerEntity);

    if (input == ECS::Components::PlayerInputType::PLAYER_UP) {
        direction.vec.y = 0;
    } else if (input == ECS::Components::PlayerInputType::PLAYER_DOWN) {
        direction.vec.y = 0;
    } else if (input == ECS::Components::PlayerInputType::PLAYER_LEFT) {
        direction.vec.x = 0;
    } else if (input == ECS::Components::PlayerInputType::PLAYER_RIGHT) {
        direction.vec.x = 0;
    }
}

void ECS::Systems::PlayerSystem::onKeyEvent(const SDL_Event &event)
{
    if (event.type == SDL_KEYUP) {
        _keyUpEvents.push_back(event.key.keysym.sym);
    }
    if (event.type == SDL_KEYDOWN) {
        _keyDownEvents.push_back(event.key.keysym.sym);
    }
}

void ECS::Systems::PlayerSystem::update()
{
    auto now = _clock.now();
    auto interval = now - _start;

    if (std::chrono::duration_cast<std::chrono::milliseconds>(interval).count() < 5.0f) return;

    _start = _clock.now();

    const auto playerEntity = *Entities.cbegin();
    const auto &input = coordinator.GetComponent<ECS::Components::PlayerComponent>(playerEntity);

    for (const auto &event : _keyDownEvents) {
        try
        {
            auto &dir = input.playerInputMap.at(event);
            handleKeyUpEvent(dir);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    for (const auto &event : _keyUpEvents) {
        try
        {
            auto &dir = input.playerInputMap.at(event);
            handleKeyDownEvent(dir);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        
    }

    auto &position = coordinator.GetComponent<ECS::Components::Position>(playerEntity);
    auto &direction = coordinator.GetComponent<ECS::Components::DirectionComponent>(playerEntity);

    if (position.pos.x + direction.vec.x * direction.velocity > 1700 || position.pos.x + direction.vec.x * direction.velocity <= 0 ||
        position.pos.y + direction.vec.y * direction.velocity > 800 || position.pos.y + direction.vec.y * direction.velocity <= 0) {
        direction.vec.x = 0;
        direction.vec.y = 0;
    }
    else {
        position.pos.x += direction.vec.x * direction.velocity;
        position.pos.y += direction.vec.y * direction.velocity;
    }

    _keyUpEvents.clear();
    _keyDownEvents.clear();
}