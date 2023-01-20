#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <memory>
#include <map>
#include <stdexcept>
#include <utility>

#include "./basic.hpp"

namespace ECS {
    namespace Components {
        
        enum PlayerInputType : uint16_t {
            PLAYER_UP,
            PLAYER_DOWN,
            PLAYER_LEFT,
            PLAYER_RIGHT,
            PLAYER_LEFT_CLICK,
        };

        struct ImageTexture {
            SDL_Texture* texture;
            int w, h;
        };

        struct PlayerComponent {
            std::map<SDL_Keycode, PlayerInputType> playerInputMap;
        };

        inline std::pair<SDL_Texture*, std::pair<int, int> > getTextureFromPath(std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> &windowRenderer, const std::string &path)
        {
            int w, h;
            SDL_Surface *surface = IMG_Load(path.c_str());
            SDL_Texture *pictureTexture = nullptr;

            if (surface == nullptr)
                throw std::runtime_error("Could not load: " + path + " : " + std::string(SDL_GetError()));

            pictureTexture = SDL_CreateTextureFromSurface(windowRenderer.get(), surface);

            if (pictureTexture == nullptr)
                throw std::runtime_error("Could not get texture: " + path + " : " + std::string(SDL_GetError()));

            SDL_QueryTexture(pictureTexture, nullptr, nullptr, &w, &h);

            SDL_FreeSurface(surface);
            return std::make_pair(pictureTexture, std::make_pair(w, h));
        }

    };
}