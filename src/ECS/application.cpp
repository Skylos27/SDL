#include "include/application.hpp"

#include "components/basic.hpp"
#include "components/sdl_components.hpp"
#include <iostream>

#include <chrono>

ECS::Coordinator coordinator;

application::MainApplication::MainApplication(const window::WindowSettings& settings)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    _window = std::make_unique<window::SDLWindow>(settings);
}

application::MainApplication::~MainApplication()
{
    SDL_Quit();
}

int application::MainApplication::loop()
{
    coordinator.init();

    coordinator.RegisterComponent<ECS::Components::ImageTexture>();
    coordinator.RegisterComponent<ECS::Components::Position>();
    coordinator.RegisterComponent<ECS::Components::CharacterType>();

    std::vector<ECS::Entity> sheeps(4);
    int x = 0;

    for (auto &sheep : sheeps) {
        sheep = coordinator.CreateEntity();

        coordinator.AddComponent<ECS::Components::Position>(
            sheep,
            ECS::Components::Position{
                ECS::Components::Vector2{x, 0},
            }
        );

        coordinator.AddComponent<ECS::Components::CharacterType>(
            sheep,
            ECS::Components::CharacterType{
                ECS::Components::CharacterEnumType::SHEEP,
            }
        );

        coordinator.AddComponent<ECS::Components::ImageTexture>(
            sheep,
            ECS::Components::ImageTexture{
                ECS::Components::getTextureFromPath(this->getRenderer(), "./assets/sheep.png"),
            }
        );

        x += 80;
    }

    bool shouldQuit = false;
    float dt = 0.0f;
    float total = 0.0f;
    auto begin = std::chrono::high_resolution_clock::now();

    while (!shouldQuit) {
        auto startTime = std::chrono::high_resolution_clock::now();
        
        for (auto &sheep : sheeps) {
            const auto &texture = coordinator.GetComponent<ECS::Components::ImageTexture>(sheep);
            const auto &position = coordinator.GetComponent<ECS::Components::Position>(sheep);
            _window->displayTexture(texture.texture, position);
        }

        _window->update();
        
        auto stopTime = std::chrono::high_resolution_clock::now();

		dt = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();
        total = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - begin).count();


        if (total > 5)
            break;
    }

    return 0;
}