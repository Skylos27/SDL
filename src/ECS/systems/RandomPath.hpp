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

        class RandomPathSystem : public System
        {
            public:
                void init();
                void update();

            private:
                std::random_device rd;
                std::mt19937 mt;
                std::uniform_int_distribution<std::uint16_t> dist;
                std::chrono::high_resolution_clock _clock;
                std::chrono::high_resolution_clock::time_point _start;
        };

    }
}