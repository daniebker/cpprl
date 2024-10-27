#pragma once

#include <fmt/core.h>
#include <fmt/format.h>

#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <libtcod.hpp>

#include "colours.hpp"

namespace cpprl {

  struct Message {
    std::string text_;
    tcod::ColorRGB colour_;
    int count_;

    Message() = default;
    Message(std::string text, tcod::ColorRGB color = WHITE, int count = 1)
      : text_(text), colour_(color), count_(count) {}

    std::string full_text() const {
      return count_ > 1 ? fmt::format("{} (x{})", text_, count_) : text_;
    }

    template <class Archive>
      void serialize(Archive& archive) {
        archive(text_, colour_, count_);
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

      template <class Archive>
        void serialize(Archive& archive) {
          // TODO: this is where we blow up
          // when using binary serialisation (tested on a mac m1)
          archive(messages_, max_messages_);
        }
  };

}

