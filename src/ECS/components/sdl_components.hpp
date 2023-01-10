#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <memory>
#include <stdexcept>

#include "./basic.hpp"

namespace ECS {
    namespace Components {
        
        struct ImageTexture {
            SDL_Texture* texture;
        };

        inline SDL_Texture* getTextureFromPath(std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> &windowRenderer, const std::string &path)
        {
            SDL_Surface *surface = IMG_Load(path.c_str());
            SDL_Texture *pictureTexture = nullptr;

            if (surface == nullptr)
                throw std::runtime_error("Could not load: " + path + " : " + std::string(SDL_GetError()));

            pictureTexture = SDL_CreateTextureFromSurface(windowRenderer.get(), surface);

            if (pictureTexture == nullptr)
                throw std::runtime_error("Could not get texture: " + path + " : " + std::string(SDL_GetError()));

            SDL_FreeSurface(surface);
            return pictureTexture;
        }

    };
}