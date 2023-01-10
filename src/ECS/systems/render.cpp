#include "./render.hpp"

extern ECS::Coordinator coordinator;

void ECS::Systems::RenderSystem::init(const std::shared_ptr<window::SDLWindow> &window)
{
    _window = window;
}

void ECS::Systems::RenderSystem::update()
{
    _window->clear();
    for (const auto &entity : Entities)
    {
        auto &texture = coordinator.GetComponent<ECS::Components::ImageTexture>(entity);
        const auto &position = coordinator.GetComponent<ECS::Components::Position>(entity);

        _window->displayTexture(texture.texture, position);
    }
    _window->update();
}