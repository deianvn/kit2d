#include "../../include/kit2d/core/Flags.hpp"
#include "../../include/kit2d/core/SDL2.hpp"

namespace kit2d {

  const uint32_t Flags::INIT_TIMER = SDL_INIT_TIMER;
  const uint32_t Flags::INIT_AUDIO = SDL_INIT_AUDIO;
  const uint32_t Flags::INIT_VIDEO = SDL_INIT_VIDEO;
  const uint32_t Flags::INIT_JOYSTICK = SDL_INIT_JOYSTICK;
  const uint32_t Flags::INIT_HAPTIC = SDL_INIT_HAPTIC;
  const uint32_t Flags::INIT_GAMECONTROLLER = SDL_INIT_GAMECONTROLLER;
  const uint32_t Flags::INIT_EVENTS = SDL_INIT_EVENTS;
  const uint32_t Flags::INIT_EVERYTHING = SDL_INIT_EVERYTHING;
  const uint32_t Flags::INIT_NOPARACHUTE = SDL_INIT_NOPARACHUTE;
  const uint32_t Flags::WINDOW_FULLSCREEN = SDL_WINDOW_FULLSCREEN;
  const uint32_t Flags::WINDOW_FULLSCREEN_DESKTOP = SDL_WINDOW_FULLSCREEN_DESKTOP;
  const uint32_t Flags::WINDOW_OPENGL = SDL_WINDOW_OPENGL;
  const uint32_t Flags::WINDOW_SHOWN = SDL_WINDOW_SHOWN;
  const uint32_t Flags::WINDOW_HIDDEN = SDL_WINDOW_HIDDEN;
  const uint32_t Flags::WINDOW_BORDERLESS = SDL_WINDOW_BORDERLESS;
  const uint32_t Flags::WINDOW_RESIZABLE = SDL_WINDOW_RESIZABLE;
  const uint32_t Flags::WINDOW_MINIMIZED = SDL_WINDOW_MINIMIZED;
  const uint32_t Flags::WINDOW_MAXIMIZED = SDL_WINDOW_MAXIMIZED;
  const uint32_t Flags::WINDOW_INPUT_GRABBED = SDL_WINDOW_INPUT_GRABBED;
  const uint32_t Flags::WINDOW_INPUT_FOCUS = SDL_WINDOW_INPUT_FOCUS;
  const uint32_t Flags::WINDOW_MOUSE_FOCUS = SDL_WINDOW_MOUSE_FOCUS;
  const uint32_t Flags::WINDOW_FOREIGN = SDL_WINDOW_FOREIGN;
  const uint32_t Flags::WINDOW_ALLOW_HIGHDPI = SDL_WINDOW_ALLOW_HIGHDPI;
  const uint32_t Flags::WINDOW_MOUSE_CAPTURE = SDL_WINDOW_MOUSE_CAPTURE;
  const uint32_t Flags::WINDOW_ALWAYS_ON_TOP = SDL_WINDOW_ALWAYS_ON_TOP;
  const uint32_t Flags::WINDOW_SKIP_TASKBAR = SDL_WINDOW_SKIP_TASKBAR;
  const uint32_t Flags::WINDOW_UTILITY = SDL_WINDOW_UTILITY;
  const uint32_t Flags::WINDOW_TOOLTIP = SDL_WINDOW_TOOLTIP;
  const uint32_t Flags::WINDOW_POPUP_MENU = SDL_WINDOW_POPUP_MENU;

  const int Flags::WINDOWPOS_CENTERED = SDL_WINDOWPOS_CENTERED;
  const int Flags::WINDOWPOS_UNDEFINED = SDL_WINDOWPOS_UNDEFINED;
}
