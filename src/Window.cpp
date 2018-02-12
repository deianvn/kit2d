#include <iostream>
#include "Window.hpp"
#include "../include/Err.hpp"
#include "SDL2.hpp"

namespace kit2d {

  Window::Window(std::string title,
    int x, int y,
    int width, int height,
    Uint32 flags) {

    sdlWindow = SDL_CreateWindow(title.c_str(),
      x, y,
      width, height,
      flags);

    if (sdlWindow == nullptr) {
      throw Err {};
    }
  }

  Window::~Window() {
    SDL_DestroyWindow(sdlWindow);
  }

  void Window::loop() {
    bool running = true;
    SDL_Event event;
    while (running) {
      SDL_PollEvent(&event);
      if (event.type == SDL_QUIT) {
        running = false;
      }
    }
  }

}
