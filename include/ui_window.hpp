#ifndef INCLUDE_UI_WINDOW_HPP_
#define INCLUDE_UI_WINDOW_HPP_

#include <libtcod.hpp>
#include <memory>

#include "types/math.hpp"

class TCODConsole;

namespace cpprl {
class UiWindow {
 protected:
  Vector2D position_;
  int width_, height_;

 public:
  UiWindow(std::size_t width, std::size_t height, Vector2D position);

  virtual void render(tcod::Console& console) const;
};
}  // namespace cpprl

#endif  // INCLUDE_UI_WINDOW_HPP_
