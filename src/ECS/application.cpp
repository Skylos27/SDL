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
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<std::uint16_t> dist(ECS::Components::CharacterGender::MALE, ECS::Components::CharacterGender::FEMALE);
    // std::uniform_real_distribution<float> distF(0.2, 0.3);
    std::uniform_real_distribution<float> distF(0.5f, 0.75f);
    
    coordinator.init();

    coordinator.RegisterComponent<ECS::Components::ImageTexture>();
    coordinator.RegisterComponent<ECS::Components::Position>();
    coordinator.RegisterComponent<ECS::Components::CharacterType>();
    coordinator.RegisterComponent<ECS::Components::CharacterInfo>();
    coordinator.RegisterComponent<ECS::Components::AITypeComponent>();
    coordinator.RegisterComponent<ECS::Components::DirectionComponent>();
    coordinator.RegisterComponent<ECS::Components::PlayerComponent>();
    coordinator.RegisterComponent<ECS::Components::DogBoundary>();

    std::vector<ECS::Entity> sheeps(_window->getSettings().sheep());
    std::vector<ECS::Entity> wolves(_window->getSettings().wolves());
    ECS::Entity dog;
    int x = 0;

    auto addSheepLambda = [this, &dist, &distF, &mt](float x, float y)  {
        ECS::Entity entity = coordinator.CreateEntity();
        coordinator.AddComponent<ECS::Components::Position>(
            entity,
            ECS::Components::Position{
                ECS::Components::Vector2F{x, y},
            }
        );

        coordinator.AddComponent<ECS::Components::CharacterType>(
            entity,
            ECS::Components::CharacterType{
                ECS::Components::CharacterEnumType::SHEEP,
            }
        );

        auto imageData = ECS::Components::getTextureFromPath(this->getRenderer(), "./assets/sheep.png");

        coordinator.AddComponent<ECS::Components::ImageTexture>(
            entity,
            ECS::Components::ImageTexture{
                imageData.first,
                imageData.second.first, imageData.second.second,
            }
        );

        coordinator.AddComponent<ECS::Components::CharacterInfo>(
            entity,
            ECS::Components::CharacterInfo{
                ECS::Components::CharacterGender(dist(mt)),
                std::chrono::high_resolution_clock::now(),
                std::chrono::high_resolution_clock::now(),
                std::chrono::high_resolution_clock::now(),
            }
        );

        coordinator.AddComponent<ECS::Components::AITypeComponent>(
            entity,
            ECS::Components::AITypeComponent{
                ECS::Components::AIType::PASSIVE,
            }
        );

        float vel = distF(mt);

        coordinator.AddComponent<ECS::Components::DirectionComponent>(
            entity,
            ECS::Components::DirectionComponent{
                ECS::Components::Vector2{1, 1},
                vel,
                vel,
            }
        );

        std::cout << "Entity : " << entity << " gender : " << coordinator.GetComponent<ECS::Components::CharacterInfo>(entity).gender << std::endl;

        return entity;
    };

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
		signature.set(coordinator.GetComponentType<ECS::Components::ImageTexture>());
		coordinator.SetSystemSignature<ECS::Systems::AISystem>(signature);
	}

	AISystem->init(_window->getSettings().width(), _window->getSettings().length(), addSheepLambda);

    auto randomPathSystem = coordinator.RegisterSystem<ECS::Systems::RandomPathSystem>();
	{
		ECS::Signature signature;
		signature.set(coordinator.GetComponentType<ECS::Components::DirectionComponent>());
		signature.set(coordinator.GetComponentType<ECS::Components::AITypeComponent>());
		coordinator.SetSystemSignature<ECS::Systems::RandomPathSystem>(signature);
	}

	randomPathSystem->init();

    auto dangerPathSystem = coordinator.RegisterSystem<ECS::Systems::DangerPathSystem>();
	{
		ECS::Signature signature;
		signature.set(coordinator.GetComponentType<ECS::Components::DirectionComponent>());
		signature.set(coordinator.GetComponentType<ECS::Components::CharacterInfo>());
		signature.set(coordinator.GetComponentType<ECS::Components::Position>());
		signature.set(coordinator.GetComponentType<ECS::Components::CharacterType>());
		signature.set(coordinator.GetComponentType<ECS::Components::AITypeComponent>());
		coordinator.SetSystemSignature<ECS::Systems::DangerPathSystem>(signature);
	}

	dangerPathSystem->init();

    auto speedSystem = coordinator.RegisterSystem<ECS::Systems::SpeedSystem>();
	{
		ECS::Signature signature;
		signature.set(coordinator.GetComponentType<ECS::Components::DirectionComponent>());
		signature.set(coordinator.GetComponentType<ECS::Components::CharacterInfo>());
		coordinator.SetSystemSignature<ECS::Systems::SpeedSystem>(signature);
	}

	speedSystem->init();

    auto gameStateSystem = coordinator.RegisterSystem<ECS::Systems::GameStateSystem>();
	{
		ECS::Signature signature;
		signature.set(coordinator.GetComponentType<ECS::Components::CharacterType>());
		coordinator.SetSystemSignature<ECS::Systems::GameStateSystem>(signature);
	}

	gameStateSystem->init();

    auto playerSystem = coordinator.RegisterSystem<ECS::Systems::PlayerSystem>();
	{
		ECS::Signature signature;
		signature.set(coordinator.GetComponentType<ECS::Components::PlayerComponent>());
		signature.set(coordinator.GetComponentType<ECS::Components::DirectionComponent>());
		signature.set(coordinator.GetComponentType<ECS::Components::Position>());
		signature.set(coordinator.GetComponentType<ECS::Components::ImageTexture>());
		signature.set(coordinator.GetComponentType<ECS::Components::CharacterType>());
		coordinator.SetSystemSignature<ECS::Systems::PlayerSystem>(signature);
	}

	playerSystem->init();

    auto x2 = 0;

    auto playerEntity = coordinator.CreateEntity();
    {
        coordinator.AddComponent<ECS::Components::Position>(
            playerEntity,
            ECS::Components::Position{
                ECS::Components::Vector2F{static_cast<float>(x2 + 150), 150.0f},
            }
        );

        coordinator.AddComponent<ECS::Components::CharacterType>(
            playerEntity,
            ECS::Components::CharacterType{
                ECS::Components::CharacterEnumType::PLAYER,
            }
        );

        auto imageData = ECS::Components::getTextureFromPath(this->getRenderer(), "./assets/shepherd.png");

        coordinator.AddComponent<ECS::Components::ImageTexture>(
            playerEntity,
            ECS::Components::ImageTexture{
                imageData.first,
                imageData.second.first, imageData.second.second,
            }
        );

        coordinator.AddComponent<ECS::Components::DirectionComponent>(
            playerEntity,
            ECS::Components::DirectionComponent{
                ECS::Components::Vector2{0, 0},
                0.75f,
                0.75f,
            }
        );

        std::map<SDL_Keycode, ECS::Components::PlayerInputType> inputs = {
                { SDLK_UP, ECS::Components::PlayerInputType::PLAYER_UP },
                { SDLK_DOWN, ECS::Components::PlayerInputType::PLAYER_DOWN },
                { SDLK_RIGHT, ECS::Components::PlayerInputType::PLAYER_RIGHT },
                { SDLK_LEFT, ECS::Components::PlayerInputType::PLAYER_LEFT },
            };

        coordinator.AddComponent<ECS::Components::PlayerComponent>(
            playerEntity,
            ECS::Components::PlayerComponent{
                inputs
            }
        );
    }

    auto dogPathSystem = coordinator.RegisterSystem<ECS::Systems::DogPathSystem>();
	{
		ECS::Signature signature;
		signature.set(coordinator.GetComponentType<ECS::Components::DogBoundary>());
		signature.set(coordinator.GetComponentType<ECS::Components::DirectionComponent>());
		signature.set(coordinator.GetComponentType<ECS::Components::Position>());
		signature.set(coordinator.GetComponentType<ECS::Components::ImageTexture>());
		signature.set(coordinator.GetComponentType<ECS::Components::CharacterType>());
		coordinator.SetSystemSignature<ECS::Systems::DogPathSystem>(signature);
	}

	dogPathSystem->init(playerEntity);

    for (auto &wolf : wolves) {
        wolf = coordinator.CreateEntity();

        coordinator.AddComponent<ECS::Components::Position>(
            wolf,
            ECS::Components::Position{
                ECS::Components::Vector2F{static_cast<float>(x2), static_cast<float>(_window->getSettings().length() - 50)},
            }
        );

        coordinator.AddComponent<ECS::Components::CharacterType>(
            wolf,
            ECS::Components::CharacterType{
                ECS::Components::CharacterEnumType::WOLF,
            }
        );

        auto imageData = ECS::Components::getTextureFromPath(this->getRenderer(), "./assets/wolf.png");

        coordinator.AddComponent<ECS::Components::ImageTexture>(
            wolf,
            ECS::Components::ImageTexture{
                imageData.first,
                imageData.second.first, imageData.second.second,
            }
        );

        coordinator.AddComponent<ECS::Components::CharacterInfo>(
            wolf,
            ECS::Components::CharacterInfo{
                ECS::Components::CharacterGender(dist(mt)),
                std::chrono::high_resolution_clock::now(),
                std::chrono::high_resolution_clock::now(),
                std::chrono::high_resolution_clock::now(),
            }
        );

        coordinator.AddComponent<ECS::Components::AITypeComponent>(
            wolf,
            ECS::Components::AITypeComponent{
                ECS::Components::AIType::AGGRESIVE,
            }
        );

        float vel = distF(mt);

        coordinator.AddComponent<ECS::Components::DirectionComponent>(
            wolf,
            ECS::Components::DirectionComponent{
                ECS::Components::Vector2{1, -1},
                vel,
                vel,
                false,
                std::chrono::high_resolution_clock::now(),
                0.2f,
            }
        );

        x2 += 1000;
    }

    float ySheep = 0; 
    for (auto &sheep : sheeps) {
        sheep = addSheepLambda(x, ySheep);

        x += 300;
        if (x >= _window->getSettings().width()) {
            x = 0;
            ySheep += 100;
        }
    }

    auto dogEntity = coordinator.CreateEntity();
    {
        const auto &playerPosition = coordinator.GetComponent<ECS::Components::Position>(playerEntity);

        coordinator.AddComponent<ECS::Components::Position>(
            dogEntity,
            ECS::Components::Position{
                ECS::Components::Vector2F{playerPosition.pos.x - 100, playerPosition.pos.y - 100},
            }
        );

        coordinator.AddComponent<ECS::Components::CharacterType>(
            dogEntity,
            ECS::Components::CharacterType{
                ECS::Components::CharacterEnumType::DOG,
            }
        );

        coordinator.AddComponent<ECS::Components::CharacterInfo>(
            dogEntity,
            ECS::Components::CharacterInfo{
                ECS::Components::CharacterGender(dist(mt)),
                std::chrono::high_resolution_clock::now(),
                std::chrono::high_resolution_clock::now(),
                std::chrono::high_resolution_clock::now(),
            }
        );

        auto imageData = ECS::Components::getTextureFromPath(this->getRenderer(), "./assets/shepherd_dog.png");

        coordinator.AddComponent<ECS::Components::ImageTexture>(
            dogEntity,
            ECS::Components::ImageTexture{
                imageData.first,
                imageData.second.first, imageData.second.second,
            }
        );

        coordinator.AddComponent<ECS::Components::DirectionComponent>(
            dogEntity,
            ECS::Components::DirectionComponent{
                ECS::Components::Vector2{1, 0},
                1.5,
                1.5,
            }
        );

        coordinator.AddComponent<ECS::Components::DogBoundary>(
            dogEntity,
            ECS::Components::DogBoundary{
                ECS::Components::Vector2F{200, 250},
                ECS::Components::Vector2F{0, 0},
            }
        );

        coordinator.AddComponent<ECS::Components::AITypeComponent>(
            dogEntity,
            ECS::Components::AITypeComponent{
                ECS::Components::AIType::FOLLOW,
            }
        );

    }

    bool shouldQuit = false;
    SDL_Event event;
    std::chrono::high_resolution_clock clock;
    std::chrono::high_resolution_clock::time_point start = clock.now();


    while (!shouldQuit) {
        auto now = clock.now();
        auto interval = now - start;
        
        if (std::chrono::duration_cast<std::chrono::seconds>(interval).count() >=  _window->getSettings().time()) break;

        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT)
            break;
        playerSystem->onKeyEvent(event);

        try
        {
            randomPathSystem->update();
            dangerPathSystem->update();
            speedSystem->update();
            playerSystem->update();
            dogPathSystem->update();
            AISystem->update();
            renderSystem->update();
            shouldQuit = gameStateSystem->gameShouldEnd();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    return 0;
}