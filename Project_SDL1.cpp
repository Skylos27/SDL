// SDL_Test.cpp: Definiert den Einstiegspunkt für die Anwendung.
//

#include "Project_SDL1.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <numeric>
#include <random>
#include <string>

void init() {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0)
    throw std::runtime_error("init():" + std::string(SDL_GetError()));

  // Initialize PNG loading
  int imgFlags = IMG_INIT_PNG;
  if (!(IMG_Init(imgFlags) & imgFlags))
    throw std::runtime_error("init(): SDL_image could not initialize! "
                             "SDL_image Error: " +
                             std::string(IMG_GetError()));
}

namespace {
// Defining a namespace without a name -> Anonymous workspace
// Its purpose is to indicate to the compiler that everything
// inside of it is UNIQUELY used within this source file.

SDL_Surface* load_surface_for(const std::string& path,
                              SDL_Surface* window_surface_ptr) {
  SDL_Surface *surface = IMG_Load(path.c_str());
  SDL_Surface *pictureTexture = nullptr;

  if (surface == nullptr)
      throw std::runtime_error("Could not load: " + path + " : " + std::string(SDL_GetError()));

  pictureTexture = SDL_ConvertSurface(surface, window_surface_ptr->format, 0);

  if (pictureTexture == nullptr)
      throw std::runtime_error("Could not get texture: " + path + " : " + std::string(SDL_GetError()));


  SDL_FreeSurface(surface);
  return pictureTexture;
  // Helper function to load a png for a specific surface
  // See SDL_ConvertSurface
}
} // namespace


animal::animal(const std::string& file_path, SDL_Surface* window_surface_ptr) : window_surface_ptr_(window_surface_ptr)
{
   image_ptr_ = load_surface_for(file_path, window_surface_ptr);
   position_.h = 80;
   position_.w = 80;
}

animal::~animal()
{
  if (image_ptr_ != nullptr)
    SDL_FreeSurface(image_ptr_);
}

void animal::setPosition(const int x, const int y)
{
  position_.x = x;
  position_.y = x;
}

SDL_Rect &animal::getPosition()
{
  return position_;
}

int &animal::getDirectionX()
{
  return directionX;
}

int &animal::getDirectionY()
{
  return directionY;
}

void animal::draw()
{
  SDL_BlitScaled(image_ptr_, nullptr, window_surface_ptr_, &position_);
}

sheep::sheep(const std::string& file_path, SDL_Surface* window_surface_ptr, const int posX, const int posY) : animal(file_path, window_surface_ptr)
{
  getPosition().x = posX;
  getPosition().y = posY;
}

sheep::~sheep()
{
}

void sheep::move()
{
  if (getPosition().x + frame_boundary > frame_width) {
    getDirectionX() = -1;
  } else if (getPosition().x < frame_boundary) {
    getDirectionX() = 1;
  }

  if (getPosition().y + frame_boundary > frame_height) {
    getDirectionY() = -1;
  } else if (getPosition().y < frame_boundary) {
    getDirectionY() = 1;
  }

  getPosition().x += sheepVelocity * getDirectionX();
  getPosition().y += sheepVelocity * getDirectionY();
}

wolf::wolf(const std::string& file_path, SDL_Surface* window_surface_ptr, const int posX, const int posY) : animal(file_path, window_surface_ptr)
{
  getPosition().x = posX;
  getPosition().y = posY;
}

wolf::~wolf()
{
}

void wolf::move()
{
  if (getPosition().x + frame_boundary > frame_width) {
    getDirectionX() = -1;
  } else if (getPosition().x < frame_boundary) {
    getDirectionX() = 1;
  }

  if (getPosition().y + frame_boundary > frame_height) {
    getDirectionY() = -1;
  } else if (getPosition().y < frame_boundary) {
    getDirectionY() = 1;
  }

  getPosition().x += wolfVelocity * getDirectionX();
  getPosition().y += wolfVelocity * getDirectionY();
}

ground::ground(SDL_Surface* window_surface_ptr) : window_surface_ptr_(window_surface_ptr), animals_()
{
  animals_.reserve(10);
}

ground::~ground()
{
  for (auto x : animals_)
    delete x;
}

void ground::add_animal(animal *animal)
{
  animals_.push_back(animal);
}

void ground::update()
{
  SDL_FillRect(window_surface_ptr_, nullptr, SDL_MapRGB(window_surface_ptr_->format, 0, 255, 0));
  for (auto animal : animals_)
  {
    animal->move();
    animal->draw();
  }
  // SDL_UpdateWindowSurface(window_surface_ptr_);
}

application::application(unsigned n_sheep, unsigned n_wolf)
{
  window_ptr_ = SDL_CreateWindow("SDL Part 1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, frame_width, frame_height, 0);
  window_surface_ptr_ = SDL_GetWindowSurface(window_ptr_);
  ground_ = new ground(window_surface_ptr_);

  int posX = frame_boundary;
  int posY = frame_boundary;

  for (unsigned i = 0; i < n_sheep; i++)
  {
    ground_->add_animal(new sheep("./media/sheep.png", window_surface_ptr_, posX, posY));
    posX += 100;
  }

  for (unsigned i = 0; i < n_wolf; i++)
  {
    ground_->add_animal(new wolf("./media/wolf.png", window_surface_ptr_, posX, posY));
    posX += 100;
  }
}

application::~application()
{
  SDL_FreeSurface(window_surface_ptr_);
  SDL_DestroyWindow(window_ptr_);
}

int application::loop(unsigned period)
{
  unsigned int lastTime = 0, currentTime;
  bool quit = false;

  while (!quit)
  {
    while (SDL_PollEvent(&window_event_)) 
    {
      if (window_event_.type == SDL_QUIT) quit = true;
    }
    currentTime = SDL_GetTicks();
    if (currentTime > period * 1000)
      break;
    if (currentTime > lastTime + frame_rate) {
      ground_->update();
      SDL_UpdateWindowSurface(window_ptr_);
      lastTime = currentTime;
    }
  }

  return 0;
}