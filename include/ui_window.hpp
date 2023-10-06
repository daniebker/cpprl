#ifndef INCLUDE_UI_WINDOW_HPP_
#define INCLUDE_UI_WINDOW_HPP_

#include <libtcod.hpp>
#include <memory>

#include "types/math.hpp"

class TCODConsole;

namespace cpprl {
class UiWindow {
 protected:
  int width_, height_;
  Vector2D position_;
  std::unique_ptr<TCODConsole> console_;

 public:
  UiWindow(std::size_t width, std::size_t height, Vector2D position);

  virtual void render(tcod::Console& parent_console) const;
};
}  // namespace cpprl

#endif  // INCLUDE_UI_WINDOW_HPP_
