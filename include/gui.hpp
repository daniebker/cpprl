#ifndef INCLUDE_HISTORY_WINDOW_HPP_
#define INCLUDE_HISTORY_WINDOW_HPP_

#include <libtcod.hpp>
#include <map>
#include <memory>

#include "game_entity.hpp"
#include "message_log.hpp"
#include "types/math.hpp"

namespace cpprl {
static constexpr int BORDER_TOP_LEFT = 0x250C;  // '┌' in Unicode
static constexpr int BORDER_HORIZONTAL = 0x2500;  // '─' in Unicode
static constexpr int BORDER_TOP_RIGHT = 0x2510;  // '┐' in Unicode
static constexpr int BORDER_VERTICAL = 0x2502;  // '│' in Unicode
static constexpr int BORDER_SPACE = 0x0020;  // Space character
static constexpr int BORDER_BOTTOM_LEFT = 0x2514;  // '└' in Unicode
static constexpr int BORDER_BOTTOM_RIGHT = 0x2518;  // '┘' in Unicode cursor_e
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

class UiWindow {
 protected:
  int width_;
  int height_;
  Vector2D position_;
  std::unique_ptr<TCODConsole> console_;
  int cursor_;
  std::string title_;

 public:
  UiWindow(
      std::size_t width,
      std::size_t height,
      Vector2D position,
      std::string title = "")
      : width_(width),
        height_(height),
        position_(position),
        console_(new TCODConsole(width, height)),
        cursor_(0),
        title_(title){};
  virtual ~UiWindow() = default;

  virtual void add_frame() const;
  virtual void render(tcod::Console& parent_console);
  void set_cursor(int cursor) {
    if (cursor < 0) {
      cursor_ = 0;
    } else {
      cursor_ = cursor;
    }
  }
  int get_cursor() const { return cursor_; }
};

class HistoryWindow : public UiWindow {
 private:
  MessageLog& message_log_;
  int log_size_;

 public:
  HistoryWindow(
      std::size_t width,
      std::size_t height,
      Vector2D position,
      MessageLog& message_log,
      std::string title = "")
      : UiWindow(width, height, position, title),
        message_log_(message_log),
        log_size_(message_log_.get_messages().size()){};

  virtual void render(tcod::Console& parent_console) override;
};

class InventoryWindow final : public UiWindow {
 private:
  Entity* entity_;

 public:
  InventoryWindow(
      std::size_t width,
      std::size_t height,
      Vector2D position,
      Entity* entity,
      std::string title = "")
      : UiWindow(width, height, position, title), entity_(entity) {
    UiWindow::set_cursor(1);
  };

  virtual void render(tcod::Console& parent_console) override;
};

class CharacterMenuWindow final : public UiWindow {
 private:
  Entity* entity_;

 public:
  CharacterMenuWindow(
      std::size_t width,
      std::size_t height,
      Vector2D position,
      Entity* entity,
      std::string title = "")
      : UiWindow(width, height, position, title), entity_(entity) {
    UiWindow::set_cursor(1);
  };

  virtual void render(tcod::Console& parent_console) override;
};

class GameOverWindow final : public UiWindow {
 private:
  std::string message_;

 public:
  GameOverWindow(
      std::size_t width,
      std::size_t height,
      Vector2D position,
      std::string message,
      std::string title = "")
      : UiWindow(width, height, position, title), message_(message) {
    UiWindow::set_cursor(1);
  };
  virtual void render(tcod::Console& parent_console) override;
};

class MainMenuWindow final : public UiWindow {
 private:
  // This could be a map of MenuItems
  // Where menu item is a name and a
  // sub command
  std::map<int, std::string> menu_items_;

 public:
  MainMenuWindow(
      std::size_t width,
      std::size_t height,
      Vector2D position,
      std::string title = "Main Menu")
      : UiWindow(width, height, position, title) {
    menu_items_[1] = "New Game";
    menu_items_[2] = "Continue";
    menu_items_[3] = "Quit";
    UiWindow::set_cursor(1);
  }
  void render(tcod::Console& parent_console) override;
};

}  // namespace cpprl

#endif
