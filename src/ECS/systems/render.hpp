#pragma once

#include <SDL.h>

#include "../include/system.hpp"
#include "../include/window.hpp"
#include "../include/coordinator.hpp"
#include "../components/sdl_components.hpp"
#include "../components/basic.hpp"

namespace ECS {
    namespace Systems {

        class RenderSystem : public System
        {
            public:
                void init(const std::shared_ptr<window::SDLWindow> &window);
                void update();

            private:
                std::shared_ptr<window::SDLWindow> _window; 
        };

    }
}