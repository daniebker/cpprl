#pragma once

#include "event.hpp"
#include "types.hpp"
#include <functional>
#include <list>
#include <unordered_map>

namespace SupaRL
{
  class EventManager
  {
    public:
      void add_listener(EventId eventId, std::function<void(Event&)> const& listener)
      {
        listeners_[eventId].push_back(listener);
      }

      void send_event(Event& event)
      {
        uint32_t type = event.get_type();

        for (auto const& listener : listeners_[type])
        {
          listener(event);
        }
      }

      void send_event(EventId eventId)
      {
        Event event(eventId);

        for (auto const& listener : listeners_[eventId])
        {
          listener(event);
        }
      }

    private:
      std::unordered_map<EventId, std::list<std::function<void(Event&)>>> listeners_;
  };
}
