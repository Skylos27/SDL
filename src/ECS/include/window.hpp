#pragma once

#include "SDL.h"
#include "../components/basic.hpp"
#include <utility>
#include <memory>
#include <string>

namespace window {
	class WindowSettings
	{
		public:
			explicit WindowSettings(const uint16_t width, const uint16_t length, const std::string &title, const uint16_t wolves, const uint16_t sheep, const uint16_t time);
            explicit WindowSettings(const WindowSettings &other);
			WindowSettings() = delete;

			~WindowSettings() = default;

            WindowSettings& operator=(WindowSettings &&other)
            {
                return *this;
            }

			uint16_t width() const
			{
				return _width;
			}

			uint16_t length() const
			{
				return _length;
			}

            std::string title() const
            {
                return _title;
            }

            uint16_t wolves() const
			{
				return _wolves;
			}

            uint16_t sheep() const
			{
				return _sheep;
			}

            uint16_t time() const
			{
				return _time;
			}

		private:
			uint16_t _width;
			uint16_t _length;
            std::string _title;
            uint16_t _wolves;
            uint16_t _sheep;
            uint16_t _time;
	};

    class SDLWindow
    {
        public:
            explicit SDLWindow(const WindowSettings &settings);
            SDLWindow() = delete;
            ~SDLWindow() = default;

            void displayTexture(SDL_Texture* texture, const ECS::Components::Position &position, const std::pair<int, int> &size);
            void update();
            void clear();

            std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> &getRenderer()
            {
                return _renderer;
            }

            const WindowSettings &getSettings() const
            {
                return _settings;
            }

        private:
            WindowSettings _settings;
            std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> _window;
            std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> _renderer;
    };
    
}
