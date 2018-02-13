#include "../include/Window.hpp"
#include "../include/Err.hpp"
#include "../include/SDL2.hpp"

#include <iostream>

namespace kit2d {

  Window::Window(const char* title,
    int x, int y,
    int width, int height,
    Uint32 flags) {

    sdlWindow = SDL_CreateWindow(title,
      x, y,
      width, height,
      flags);

    if (sdlWindow == nullptr) {
      throw Err {};
    }
    sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);
    if (sdlRenderer == nullptr) {
      throw Err {};
    }
  }

  Window::~Window() {
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(sdlWindow);
  }

  void Window::onRender(OnRenderCallback onRenderCallback) {
    this->onRenderCallback = onRenderCallback;
  }

  void Window::onUpdate(OnUpdateCallback onUpdateCallback) {
    this->onUpdateCallback = onUpdateCallback;
  }

  void Window::loop() {
    SDL_Event event;
    Renderer r = renderer();
    bool running = true;
    uint64_t currentTime = SDL_GetPerformanceCounter(), lastTime;

    while (running) {
      while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
          running = false;
        }
      }
      lastTime = currentTime;
      currentTime = SDL_GetPerformanceCounter();
      if (onUpdateCallback != nullptr) {
        onUpdateCallback(static_cast<float>(currentTime - lastTime) / SDL_GetPerformanceFrequency());
      }
      if (onRenderCallback != nullptr) {
        onRenderCallback(r);
      }
    }
  }

  const Renderer Window::renderer() {
    return Renderer { sdlRenderer };
  }

  Texture Window::loadTexture(const char *path) {
    SDL_Texture* sdlTexture = IMG_LoadTexture(sdlRenderer, path);
    if (sdlTexture == nullptr) {
      throw Err {};
    }
    return Texture { sdlTexture };
  }

}
