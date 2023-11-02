#include "message_log.hpp"

#include "colours.hpp"

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
    std::string text, tcod::ColorRGB color, bool stack) {
  add_message(Message{text, color, 1}, stack);
}

void MessageLog::render(
    tcod::Console& console, int x, int y, int width, int height) const {
  render_messages(console, x, y, width, height);
}

// TODO: Bug where it's not overlaying correctly
void MessageLog::render_messages(
    tcod::Console& console, int x, int y, int width, int height) const {
  int y_offset = 0;
  for (auto it = messages_.rbegin(); it != messages_.rend(); ++it) {
    const Message& message = *it;
    int line_height = tcod::get_height_rect(width, it->full_text());

    if (line_height > 1) {
      y_offset += line_height - 1;
    }

    if (y_offset >= height) {
      break;
    }
    std::string text = message.full_text();
    tcod::print_rect(
        console,
        {x, y + y_offset, width, line_height},
        message.full_text(),
        message.colour_,
        BLACK_DARK,
        TCOD_LEFT);
    y_offset++;
  }
}

void MessageLog::save(TCODZip& zip) {
  zip.putInt(messages_.size());
  for (Message message : messages_) {
    zip.putString(message.text_.c_str());
    zip.putInt(message.colour_.r);
    zip.putInt(message.colour_.g);
    zip.putInt(message.colour_.b);
    zip.putInt(message.count_);
  }
}
void MessageLog::load(TCODZip& zip) {
  int nbMessages = zip.getInt();
  while (nbMessages > 0) {
    const char* text = zip.getString();
    int r = zip.getInt();
    int g = zip.getInt();
    int b = zip.getInt();
    tcod::ColorRGB col = {(uint8_t)r, (uint8_t)g, (uint8_t)b};
    int count = zip.getInt();
    nbMessages--;
    messages_.emplace_back(text, col, count);
  }
}

}  // namespace cpprl
