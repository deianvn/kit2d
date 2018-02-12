#ifndef KIT2D_ERR_HPP
#define KIT2D_ERR_HPP

#include <stdexcept>

namespace kit2d {

  class Err : public std::runtime_error {
  public:
    Err();
  };

}

#endif
