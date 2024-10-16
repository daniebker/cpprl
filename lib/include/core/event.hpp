#pragma once

#include "types.hpp"
#include <any>
#include <unordered_map>

namespace SupaRL {

  class Event
  {
    public:
      Event() = delete;

      explicit Event(EventId type)
        : type_(type)
      {}

      template<typename T>
        void set_param(ParamId id, T value)
        {
          data_[id] = value;
        }

      template<typename T>
        T get_param(ParamId id)
        {
          return std::any_cast<T>(data_[id]);
        }

      EventId get_type() const
      {
        return type_;
      }

    private:
      EventId type_{};
      std::unordered_map<EventId, std::any> data_{};
  };
}
