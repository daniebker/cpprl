#ifndef INCLUDE_MESSAGE_LOG_HPP_
#define INCLUDE_MESSAGE_LOG_HPP_

#include <fmt/core.h>
#include <fmt/format.h>

#include <iostream>
#include <libtcod.hpp>

#include "colours.hpp"

namespace cpprl {

struct Message {
  std::string text_;
  tcod::ColorRGB colour_;
  int count_;

  Message(std::string text, tcod::ColorRGB color = WHITE, int count = 1)
      : text_(text), colour_(color), count_(count) {}

  std::string full_text() const {
    return count_ > 1 ? fmt::format("{} (x{})", text_, count_) : text_;
  }
};

class MessageLog {
 private:
  /**
   * The stored messages.
   */
  std::vector<Message> messages_;

  /**
   * Max look back of messages we should store.
   */
  size_t max_messages_;

  /**
   * renders a list of messages in reverse order
   */
  void render_messages(
      tcod::Console& console, int x, int y, int width, int height) const;

 public:
  MessageLog(int max_messages = 1024) : max_messages_(max_messages) {
    messages_.reserve(max_messages_);
  }
  void add_message(Message message, bool stack);
  void add_message(
      std::string text, tcod::ColorRGB color = WHITE, bool stack = true);

  /**
   * Renders a log message in the given area.
   */
  void render(
      tcod::Console& console, int x, int y, int width, int height) const;
};

}  // namespace cpprl

#endif
