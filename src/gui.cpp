#include "gui.hpp"

#include <algorithm>

#include "colours.hpp"
#include "components.hpp"
#include "game_entity.hpp"

namespace cpprl {

void UiWindow::render(tcod::Console&) {
  // no op
}

void UiWindow::add_frame() const {
  tcod::draw_frame(
      *console_,
      {0, 0, console_->getWidth(), console_->getHeight()},
      LEGEND,
      WHITE,
      BLACK);

  tcod::print_rect(
      *console_,
      {0, 0, console_->getWidth(), 1},
      title_,
      WHITE,
      BLACK,
      TCOD_CENTER);
}

void HistoryWindow::render(tcod::Console& parent_console) {
  std::vector<Message> allMessages(message_log_.get_messages());
  int allMessagesIntSize = static_cast<int>(allMessages.size());
  int start = std::min(allMessagesIntSize, cursor_);

  std::vector<Message> messages =
      std::vector<Message>(allMessages.rbegin() + start, allMessages.rend());

  console_->clear();
  UiWindow::add_frame();

  int y_offset = console_->getHeight() - 1;
  for (auto it = messages.begin(); it != messages.end(); ++it) {
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
    if (y_offset <= 0) {
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

void InventoryWindow::render(tcod::Console& parent_console) {
  console_->clear();
  UiWindow::add_frame();

  std::vector<Entity*> items = entity_->get_container()->get_inventory();
  int y_offset = 1;
  if (items.size() == 0) {
    tcod::print_rect(
        *console_,
        {2, y_offset, console_->getWidth() - 1, 1},
        "Inventory is empty",
        WHITE,
        std::nullopt,
        TCOD_LEFT);
    ++y_offset;
  } else {
    tcod::print(*console_, {1, cursor_}, ">", WHITE, std::nullopt, TCOD_LEFT);
    for (auto it = items.begin(); it != items.end(); ++it) {
      tcod::print_rect(
          *console_,
          {2, y_offset, console_->getWidth() - 1, 1},
          (*it)->get_name(),
          WHITE,
          std::nullopt,
          TCOD_LEFT);
      ++y_offset;
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

void GameOverWindow::render(tcod::Console& parent_console) {
  console_->clear();
  UiWindow::add_frame();

  tcod::print_rect(
      *console_,
      {1, 1, console_->getWidth() - 1, console_->getHeight() - 1},
      message_,
      WHITE,
      std::nullopt,
      TCOD_LEFT);

  tcod::blit(
      parent_console,
      *console_,
      {3, 3},
      {position_.x, position_.y, console_->getWidth(), console_->getHeight()},
      1.0f,
      1.0f);
}
}  // namespace cpprl
