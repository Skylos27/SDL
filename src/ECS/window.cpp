#include "include/window.hpp"

#include <iostream>

// Settings

window::WindowSettings::WindowSettings(const uint16_t width, const uint16_t length, const std::string &title, const uint16_t sheep, const uint16_t wolves, const uint16_t time) 
: _width(width), _length(length), _title(title), _wolves(wolves), _sheep(sheep), _time(time)
{}

window::WindowSettings::WindowSettings(const WindowSettings &other): _width(other.width()), _length(other.length()), _title(other.title()), _wolves(other.wolves()), _sheep(other.sheep()), _time(other.time())
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

void window::SDLWindow::displayTexture(SDL_Texture* texture, const ECS::Components::Position &position, const std::pair<int, int> &size)
{
    SDL_FRect rect{position.pos.x, position.pos.y, static_cast<float>(size.first), static_cast<float>(size.second)};

    if (SDL_RenderCopyF(_renderer.get(), texture, nullptr, &rect) < 0)
        std::cerr << "SDL_RenderCopy : " << SDL_GetError() << std::endl;
}

void window::SDLWindow::update()
{
    SDL_RenderPresent(_renderer.get());
}

void window::SDLWindow::clear()
{
    SDL_RenderClear(_renderer.get());
}