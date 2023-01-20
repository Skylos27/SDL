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

        class SpeedSystem : public System
        {
            public:
                void init();
                void update();

            private:
                std::chrono::high_resolution_clock _clock;
                std::chrono::high_resolution_clock::time_point _start;
        };

    }
}