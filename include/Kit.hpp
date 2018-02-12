#ifndef KIT2D_KIT_HPP
#define KIT2D_KIT_HPP

#include <cstdint>
#include "Point.hpp"

namespace kit2d {

  class Kit {
  public:
    Kit(uint32_t flags);
    void quit();
    void delay(int milliseconds);
    int countDisplays();
    void useDisplay(int displayIndex);
    Point getDisplaySize(int displayIndex);
    Point getDisplaySize() { return getDisplaySize(displayIndex); }

    static Kit* DEFAULT;
  private:
    int displayIndex { 0 };
  };

}

#endif
