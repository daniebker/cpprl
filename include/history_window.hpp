#ifndef INCLUDE_HISTORY_WINDOW_HPP_
#define INCLUDE_HISTORY_WINDOW_HPP_

#include <libtcod.hpp>

#include "message_log.hpp"
#include "types/math.hpp"
#include "ui_window.hpp"

namespace cpprl {

class HistoryWindow : public UiWindow {
 private:
  MessageLog& message_log_;
  int log_size_;

 public:
  HistoryWindow(
      std::size_t width,
      std::size_t height,
      Vector2D position,
      MessageLog& message_log)
      : UiWindow(width - 6, height - 6, position),
        message_log_(message_log),
        log_size_(message_log_.get_messages().size()){};

  virtual void render(tcod::Console& parent_console) const;
};

}  // namespace cpprl

#endif
