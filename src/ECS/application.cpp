#include "include/application.hpp"

#include "components/basic.hpp"
#include "components/sdl_components.hpp"
#include <iostream>

#include <chrono>
#include <random>

ECS::Coordinator coordinator;

application::MainApplication::MainApplication(const window::WindowSettings& settings)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    _window = std::make_shared<window::SDLWindow>(settings);
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
    coordinator.RegisterComponent<ECS::Components::CharacterInfo>();
    coordinator.RegisterComponent<ECS::Components::AITypeComponent>();
    coordinator.RegisterComponent<ECS::Components::DirectionComponent>();

    std::vector<ECS::Entity> sheeps(8);
    int x = 0;

    auto renderSystem = coordinator.RegisterSystem<ECS::Systems::RenderSystem>();
	{
		ECS::Signature signature;
		signature.set(coordinator.GetComponentType<ECS::Components::ImageTexture>());
		signature.set(coordinator.GetComponentType<ECS::Components::Position>());
		coordinator.SetSystemSignature<ECS::Systems::RenderSystem>(signature);
	}

	renderSystem->init(_window);

    auto AISystem = coordinator.RegisterSystem<ECS::Systems::AISystem>();
	{
		ECS::Signature signature;
		signature.set(coordinator.GetComponentType<ECS::Components::CharacterType>());
		signature.set(coordinator.GetComponentType<ECS::Components::CharacterInfo>());
		signature.set(coordinator.GetComponentType<ECS::Components::AITypeComponent>());
		signature.set(coordinator.GetComponentType<ECS::Components::Position>());
		signature.set(coordinator.GetComponentType<ECS::Components::DirectionComponent>());
		coordinator.SetSystemSignature<ECS::Systems::AISystem>(signature);
	}

	AISystem->init(_window->getSettings().width(), _window->getSettings().length());

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<std::uint16_t> dist(ECS::Components::CharacterGender::MALE, ECS::Components::CharacterGender::FEMALE);

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

        coordinator.AddComponent<ECS::Components::CharacterInfo>(
            sheep,
            ECS::Components::CharacterInfo{
                ECS::Components::CharacterGender(dist(mt)),
                0,
                0,
            }
        );

        coordinator.AddComponent<ECS::Components::AITypeComponent>(
            sheep,
            ECS::Components::AITypeComponent{
                ECS::Components::AIType::PASSIVE,
            }
        );

        coordinator.AddComponent<ECS::Components::DirectionComponent>(
            sheep,
            ECS::Components::DirectionComponent{
                ECS::Components::Vector2{1, 1},
                1,
            }
        );

        x += 80;
    }

    bool shouldQuit = false;
    float dt = 0.0f;
    SDL_Event event;

    while (!shouldQuit) {
        SDL_PollEvent(&event);

        if (event.type == SDL_QUIT)
            break;

        auto startTime = std::chrono::high_resolution_clock::now();
        
        AISystem->update();
        renderSystem->update();

        auto stopTime = std::chrono::high_resolution_clock::now();

		dt = std::chrono::duration<float, std::chrono::milliseconds::period>(stopTime - startTime).count();
        std::cout << dt << std::endl;
    }

    return 0;
}