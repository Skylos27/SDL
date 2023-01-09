#include "include/window.hpp"

// Settings

window::WindowSettings::WindowSettings(const uint16_t width, const uint16_t length, const std::string &title) : _width(width), _length(length), _title(title)
{}

window::WindowSettings::WindowSettings(const WindowSettings &other): _width(other.width()), _length(other.length()), _title(other.title())
{}

// MainWindow

window::SDLWindow::SDLWindow(const WindowSettings &settings) : _settings(settings), _window(nullptr, SDL_DestroyWindow), _renderer(nullptr, SDL_DestroyRenderer)
{
    SDL_Window *window = SDL_CreateWindow(
        _settings.title().c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        _settings.width(),
        _settings.length(),
        SDL_WINDOW_RESIZABLE
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    _window.reset(window);
    _renderer.reset(renderer);
}

#include <iostream>

void window::SDLWindow::displayTexture(SDL_Texture* texture, const ECS::Components::Position &position)
{
    SDL_Rect rect{position.pos.x, position.pos.y, 0, 0};

    SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
    SDL_RenderCopy(_renderer.get(), texture, nullptr, &rect);
}

void window::SDLWindow::update()
{
    SDL_RenderPresent(_renderer.get());
}