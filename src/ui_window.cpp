#include "ui_window.hpp"

#include <libtcod.hpp>

namespace cpprl {

UiWindow::UiWindow(std::size_t width, std::size_t height, Vector2D position)
    : width_(width),
      height_(height),
      position_(position),
      console_(new TCODConsole(width, height)) {}

void UiWindow::render(tcod::Console& parent_console) const {
  // no op
}
}  // namespace cpprl
