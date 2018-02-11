#include <iostream>
#include "../include/Window.hpp"

namespace kit2d {

  Window::Window(std::string title,
    int x, int y,
    int width, int height,
    Uint32 flags) {

    sdlWindow = SDL_CreateWindow(title.c_str(),
      x, y,
      width, height,
      flags);

    if (sdlWindow == NULL) {
      std::cout << "Could not create window" << std::endl;
    }
  }

  Window::~Window() {
    SDL_DestroyWindow(sdlWindow);
    std::cout << "Destroying window" << std::endl;
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
