#pragma once

#include "window.hpp"

#include <cassert>
#include <memory>

#include "coordinator.hpp"
#include "../systems/render.hpp"
#include "../systems/AI.hpp"
#include "../systems/RandomPath.hpp"
#include "../systems/DangerPath.hpp"
#include "../systems/SpeedSystem.hpp"
#include "../systems/GameState.hpp"
#include "../systems/PlayerSystem.hpp"
#include "../systems/DogPath.hpp"

namespace application {
	class MainApplication
	{
		public:
			explicit MainApplication(const window::WindowSettings &settings);
			MainApplication() = delete;

            int loop();

			std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> &getRenderer()
			{
				return _window->getRenderer();
			}

			~MainApplication();

		private:
            std::shared_ptr<window::SDLWindow> _window;
	};
};