#include "Window.hpp"
#include "../include/Err.hpp"
#include "SDL2.hpp"

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
  }

  Window::~Window() {
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(sdlWindow);
  }

  void Window::loop() {
    bool running = true;
    SDL_Event event;
    Renderer r = renderer();
    while (running) {
      SDL_PollEvent(&event);
      if (event.type == SDL_QUIT) {
        running = false;
      }
      if (onRenderCallback != nullptr) {
        onRenderCallback(r);
      }
    }
  }

  Renderer Window::renderer() {
    Renderer renderer;
    renderer.internal_setSdlRenderer(sdlRenderer);
    return renderer;
  }

  Texture Window::loadTexture(const char *path) {
    SDL_Texture* sdlTexture = IMG_LoadTexture(sdlRenderer, path);
    if (sdlTexture == nullptr) {
      throw Err {};
    }
    Texture texture {};
    texture.internal_sdlTexture = sdlTexture;
    return texture;
  }

}
