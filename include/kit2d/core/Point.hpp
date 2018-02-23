#ifndef KIT2D_CORE_POINT_HPP
#define KIT2D_CORE_POINT_HPP

namespace kit2d {

  struct Point {
    Point() : x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) {}
    int x;
    int y;
  };

}

#endif
