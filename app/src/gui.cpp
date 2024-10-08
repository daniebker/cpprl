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

  std::vector<Entity*> items = entity_->get_container().get_inventory();
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

void MainMenuWindow::render(tcod::Console& parent_console) {
  console_->clear();
  UiWindow::add_frame();
  tcod::print(*console_, {1, cursor_}, ">", WHITE, std::nullopt, TCOD_LEFT);
  // print each menu item
  int y_offset = 1;
  for (auto it = menu_items_.begin(); it != menu_items_.end(); ++it) {
    // this is a sucky way to do this.
    if (y_offset == 2 && !std::filesystem::exists("saves/game.sav")) {
      continue;
    }

    tcod::print_rect(
        *console_,
        {2, y_offset, console_->getWidth() - 1, 1},
        it->second,
        WHITE,
        std::nullopt,
        TCOD_LEFT);
    ++y_offset;
  }

  tcod::blit(
      parent_console,
      *console_,
      {3, 3},
      {position_.x, position_.y, console_->getWidth(), console_->getHeight()},
      1.0f,
      1.0f);
}

void CharacterMenuWindow::render(tcod::Console& parent_console) {
  console_->clear();
  UiWindow::add_frame();

  tcod::print(*console_, {1, cursor_}, ">", WHITE, std::nullopt, TCOD_LEFT);

  int entity_damage_ = entity_->get_attack_component().get_damage();
  int entity_defense_ = entity_->get_defense_component().get_defense();
  auto stats = entity_->get_stats_component().value().get();

  tcod::print_rect(
      *console_,
      {2, 1, console_->getWidth() - 1, 1},
      fmt::format("Level: {}", stats.get_level()),
      // "Damage: " + entity_damage_,
      WHITE,
      std::nullopt,
      TCOD_LEFT);

  tcod::print_rect(
      *console_,
      {2, 2, console_->getWidth() - 1, 1},
      fmt::format("Stat Points: {}", stats.get_stats_points()),
      // "Damage: " + entity_damage_,
      WHITE,
      std::nullopt,
      TCOD_LEFT);

  tcod::print_rect(
      *console_,
      {2, 3, console_->getWidth() - 1, 1},
      fmt::format("Damage: {}", entity_damage_),
      // "Damage: " + entity_damage_,
      WHITE,
      std::nullopt,
      TCOD_LEFT);

  tcod::print_rect(
      *console_,
      {2, 4, console_->getWidth() - 1, 1},
      fmt::format("Defense: {}", entity_defense_),
      // "Defense: " + entity_defense_,
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
