#include "../../include/kit2d/core/Kit.hpp"
#include "../../include/kit2d/core/Err.hpp"
#include "../../include/kit2d/core/SDL2.hpp"

#include <iostream>

namespace kit2d {

  Kit* Kit::DEFAULT = nullptr;

  Kit::Kit(uint32_t flags) {
    if (Kit::DEFAULT != nullptr) {
      throw std::runtime_error { "Single instance is allowed" };
    }
    if (SDL_Init(flags) != 0)
      throw Err {};
    Kit::DEFAULT = this;
  }

  Kit::~Kit() {
    quit();
  }

  void Kit::run(KitRunnable kitRunnable) {
    kitRunnable(*this);
  }

  void Kit::quit() {
    Kit::DEFAULT = nullptr;
    SDL_Quit();
  }

  void Kit::delay(int milliseconds) {
    SDL_Delay(milliseconds);
  }

  int Kit::countDisplays() {
    return SDL_GetNumVideoDisplays();
  }

  void Kit::useDisplay(int index) {
    displayIndex = index;
  }

  const Point Kit::getDisplaySize(int index) {
    SDL_Rect rect;
    if (SDL_GetDisplayBounds(index, &rect) != 0) {
      throw Err {};
    }
    return Point { rect.x, rect.y };
  }

  const Rect Kit::getDisplayBounds(int displayIndex) {
    SDL_Rect rect;
    if (SDL_GetDisplayBounds(displayIndex, &rect) != 0) {
      throw Err {};
    }
    return Rect { rect.x, rect.y, rect.w, rect.h };
  }

}
