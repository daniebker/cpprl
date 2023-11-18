#ifndef INCLUDE_MESSAGE_LOG_HPP_
#define INCLUDE_MESSAGE_LOG_HPP_

#include <fmt/core.h>
#include <fmt/format.h>

#include <iostream>
#include <libtcod.hpp>

#include "colours.hpp"
#include "persistent.hpp"

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

class MessageLog : public Persistent {
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
  virtual ~MessageLog() = default;
  void add_message(Message message, bool stack);
  void add_message(
      std::string text, tcod::ColorRGB color = WHITE, bool stack = true);

  /**
   * Get the messages in this log
   */
  std::vector<Message> get_messages() const { return messages_; }

  /**
   * Renders a log message in the given area.
   */
  void render(
      tcod::Console& console, int x, int y, int width, int height) const;

  // void save(TCODZip& zip) override;
  // void load(TCODZip& zip) override;
  void save(cereal::JSONOutputArchive& archive) override;
  void load(cereal::JSONInputArchive& archive) override;
};

}  // namespace cpprl

#endif
