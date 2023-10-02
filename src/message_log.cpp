#include "message_log.hpp"

namespace cpprl {

void MessageLog::add_message(Message message, bool stack) {
  if (stack && !messages_.empty()) {
    Message& last_message = messages_.back();
    if (last_message.text_ == message.text_) {
      last_message.count_ += message.count_;
      return;
    }
  }

  messages_.push_back(message);
  if (messages_.size() > max_messages_) {
    messages_.erase(messages_.begin());
  }
}

void MessageLog::add_message(
    std::string_view text, tcod::ColorRGB color, bool stack) {
  add_message(Message{text, color, 1}, stack);
}

void MessageLog::render(
    tcod::Console& console, int x, int y, int width, int height) const {
  render_messages(console, x, y, width, height);
}

void MessageLog::render_messages(
    tcod::Console& console, int x, int y, int width, int height) const {
  int y_offset = 0;
  for (auto it = messages_.rbegin(); it != messages_.rend(); ++it) {
    const Message& message = *it;
    if (y_offset >= height) {
      break;
    }
    tcod::print_rect(
        console,
        {x, y + y_offset, width, height},
        message.text_,
        message.colour_,
        std::nullopt,
        TCOD_LEFT);
    y_offset += 1;
  }
}
}  // namespace cpprl
