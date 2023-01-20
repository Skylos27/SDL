#pragma once

#include <SDL.h>
#include <chrono>
#include <functional>
#include <optional>

#include "../include/system.hpp"
#include "../include/window.hpp"
#include "../include/math.hpp"
#include "../include/coordinator.hpp"
#include "../components/sdl_components.hpp"
#include "../components/basic.hpp"

namespace ECS {
    namespace Systems {

        class AISystem : public System
        {
            public:
                void init(int w, int h, const std::function<ECS::Entity(float, float)> &lambda);
                void update();

            private:
                bool entitiesOverlapped(const ECS::Entity &a, const ECS::Entity &b);
                std::optional<ECS::Components::Vector2F> findClosestSheep(const ECS::Components::Position &posWolf);
                ECS::Components::Vector2F findClosestDog();
                bool isDogClose(const ECS::Components::Position &posWolf);

            private:
                ECS::Components::Vector2 _windowSize;
                std::function<ECS::Entity(float, float)> _lambda;
                std::chrono::high_resolution_clock _clock;
                std::chrono::high_resolution_clock::time_point _start;
                
        };

    }
}