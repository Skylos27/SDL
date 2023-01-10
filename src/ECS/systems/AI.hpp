#pragma once

#include <SDL.h>
#include <chrono>

#include "../include/system.hpp"
#include "../include/window.hpp"
#include "../include/coordinator.hpp"
#include "../components/sdl_components.hpp"
#include "../components/basic.hpp"

namespace ECS {
    namespace Systems {

        class AISystem : public System
        {
            public:
                void init(int w, int h);
                void update();

            private:
                ECS::Components::Vector2 _windowSize;
                std::chrono::high_resolution_clock _clock;
                std::chrono::steady_clock::time_point _start;
        };

    }
}