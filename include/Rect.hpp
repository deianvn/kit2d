#ifndef KIT2D_RECT_HPP
#define KIT2D_RECT_HPP

struct Rect {
  Rect() :
    x(0), y(0), width(0), height(0) {}
  Rect(int x, int y, int width, int height) :
    x(x), y(y), width(width), height(height) {}
  int x;
  int y;
  int width;
  int height;
};

#endif
