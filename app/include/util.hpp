#pragma once
#include <string>

#include "components.hpp"
#include "types/math.hpp"

extern SupaRL::Coordinator g_coordinator;
namespace cpprl::util {
  template <typename T>
    inline auto find_entity_at(std::vector<T>& vec, int x, int y) {
      auto iterator = std::find_if(vec.begin(), vec.end(), [x, y](T& entity) {
          auto entity_position = g_coordinator.get_component<SupaRL::TransformComponent>(
              entity->get_id()).position_;
          return entity_position == SupaRL::Vector2D{x, y};
          });
      return iterator;
    }

  inline auto get_data_dir() -> std::filesystem::path {
    static auto root_directory =
      std::filesystem::path{"."};  // Begin at the working directory.
    while (!std::filesystem::exists(root_directory / "data")) {
      // If the current working directory is missing the data dir then it will
      // assume it exists in any parent directory.
      root_directory /= "..";
      if (!std::filesystem::exists(root_directory)) {
        throw std::runtime_error("Could not find the data directory.");
      }
    }
    return root_directory / "data";
  };

  inline std::string capitalize(const std::string& string) {
    auto ret = string;
    auto ch = ret[0];
    ret[0] = std::toupper(ch);
    return ret;
  }
}
