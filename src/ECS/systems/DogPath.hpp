#pragma once

#include <SDL.h>
#include <random>
#include <optional>

#include "../include/system.hpp"
#include "../include/window.hpp"
#include "../include/math.hpp"
#include "../include/coordinator.hpp"
#include "../components/sdl_components.hpp"
#include "../components/basic.hpp"

namespace ECS {
    namespace Systems {

        class DogPathSystem : public System
        {
            public:
                void init(const Entity &);
                void update();


            private:
                std::chrono::high_resolution_clock _clock;
                std::chrono::high_resolution_clock::time_point _start;
                Entity _player {};
        };

    }
}