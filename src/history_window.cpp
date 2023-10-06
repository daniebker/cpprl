#include "history_window.hpp"

#include "colours.hpp"

namespace cpprl {

void HistoryWindow::render(tcod::Console& parent_console) const {
  auto messages = message_log_.get_messages();

  console_->clear();

  static constexpr int BORDER_TOP_LEFT = 0x250C;  // '┌' in Unicode
  static constexpr int BORDER_HORIZONTAL = 0x2500;  // '─' in Unicode
  static constexpr int BORDER_TOP_RIGHT = 0x2510;  // '┐' in Unicode
  static constexpr int BORDER_VERTICAL = 0x2502;  // '│' in Unicode
  static constexpr int BORDER_SPACE = 0x0020;  // Space character
  static constexpr int BORDER_BOTTOM_LEFT = 0x2514;  // '└' in Unicode
  static constexpr int BORDER_BOTTOM_RIGHT = 0x2518;  // '┘' in Unicode

  static constexpr std::array<int, 9> LEGEND = {
      BORDER_TOP_LEFT,
      BORDER_HORIZONTAL,
      BORDER_TOP_RIGHT,
      BORDER_VERTICAL,
      BORDER_SPACE,
      BORDER_VERTICAL,
      BORDER_BOTTOM_LEFT,
      BORDER_HORIZONTAL,
      BORDER_BOTTOM_RIGHT};

  tcod::draw_frame(
      *console_,
      {0, 0, console_->getWidth(), console_->getHeight()},
      LEGEND,
      WHITE,
      BLACK);

  tcod::print_rect(
      *console_,
      {0, 0, console_->getWidth(), 1},
      "Message Log",
      WHITE,
      BLACK,
      TCOD_CENTER);

  int y_offset = console_->getHeight() - 1;
  for (auto it = messages.rbegin(); it != messages.rend(); ++it) {
    int line_height =
        tcod::get_height_rect(console_->getWidth(), it->full_text());

    if (line_height > 1) {
      y_offset -= line_height - 1;
    }
    {
      auto line =
          std::make_unique<TCODConsole>(console_->getWidth() - 2, line_height);

      tcod::print_rect(
          *console_,
          {1, -1 + y_offset, console_->getWidth() - 1, line_height},
          it->full_text(),
          it->colour_,
          std::nullopt,
          TCOD_LEFT);
    }
    --y_offset;
    if (y_offset < 0) {
      break;
    }
  }
  tcod::blit(
      parent_console,
      *console_,
      {3, 3},
      {position_.x, position_.y, console_->getWidth(), console_->getHeight()},
      1.0f,
      1.0f);
}
}  // namespace cpprl
