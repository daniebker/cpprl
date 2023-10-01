#include "ui_window.hpp"

#include <libtcod.hpp>

namespace cpprl {

UiWindow::UiWindow(std::size_t width, std::size_t height, Vector2D position)
    : position_(position), width_(width), height_(height) {}

void UiWindow::render(tcod::Console&) const {
  // no op
}
}  // namespace cpprl
