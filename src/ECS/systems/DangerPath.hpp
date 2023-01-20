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

        class DangerPathSystem : public System
        {
            public:
                void init();
                void update();

            private:
                std::optional<ECS::Components::Position> isWolfNear(const ECS::Components::Position &pos);
                std::optional<ECS::Components::Position> isDogNear(const ECS::Components::Position &pos);

            private:
                std::chrono::high_resolution_clock _clock;
                std::chrono::high_resolution_clock::time_point _start;
        };

    }
}