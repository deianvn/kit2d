#ifndef KIT2D_CORE_KIT_HPP
#define KIT2D_CORE_KIT_HPP

#include <cstdint>
#include <functional>
#include "Point.hpp"
#include "Rect.hpp"

namespace kit2d {

  class Kit {
  public:
    using KitRunnable = std::function<void(Kit&)>;

    Kit(uint32_t flags);
    ~Kit();
    void run(KitRunnable kitRunnable);
    void delay(int milliseconds);
    int countDisplays();
    void useDisplay(int displayIndex);
    const Point getDisplaySize(int displayIndex);
    const Point getDisplaySize() { return getDisplaySize(displayIndex); }
    const Rect getDisplayBounds(int displayIndex);
    const Rect getDisplayBounds() { return getDisplayBounds(displayIndex); }

    static Kit* DEFAULT;
  private:
    void quit();
    int displayIndex { 0 };
  };

}

#endif
