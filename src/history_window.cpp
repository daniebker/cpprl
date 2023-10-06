#include "history_window.hpp"

#include "colours.hpp"

namespace cpprl {

void HistoryWindow::render(tcod::Console& parent_console) const {
  auto messages = message_log_.get_messages();

  console_->clear();

  int y_offset = console_->getHeight() - 1;
  for (auto it = messages.rbegin(); it != messages.rend(); ++it) {
    int line_height =
        tcod::get_height_rect(console_->getWidth(), it->full_text());

    if (line_height > 1) {
      y_offset -= line_height - 1;
    }
    {
      auto line =
          std::make_unique<TCODConsole>(console_->getWidth(), line_height);

      tcod::print_rect(
          *console_,
          {0, 0 + y_offset, console_->getWidth(), line_height},
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
      {4, -4},
      {0, 0, console_->getWidth(), console_->getHeight()},
      1.0f,
      0.2f);
}
}  // namespace cpprl
