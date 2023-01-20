#pragma once

#include <SDL.h>
#include <random>

#include "../include/system.hpp"
#include "../include/window.hpp"
#include "../include/coordinator.hpp"
#include "../components/sdl_components.hpp"
#include "../components/basic.hpp"

namespace ECS {
    namespace Systems {

        class PlayerSystem : public System
        {
            public:
                void init();
                void update();
                void onKeyEvent(const SDL_Event &);
                void handleKeyUpEvent(const ECS::Components::PlayerInputType &);
                void handleKeyDownEvent(const ECS::Components::PlayerInputType &);

            private:
                std::chrono::high_resolution_clock _clock;
                std::chrono::high_resolution_clock::time_point _start;
                std::vector<SDL_Keycode> _keyUpEvents;
                std::vector<SDL_Keycode> _keyDownEvents;
        };

    }
}