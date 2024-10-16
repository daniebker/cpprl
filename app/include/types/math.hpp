#pragma once

#include <array>

#include <cmath>
#include <core/math.hpp>

namespace cpprl {

  struct Quadrilateral {
    operator const std::array<int, 4>() const noexcept {
      return {position.x, position.y, width, height};
    }

    bool operator==(const Quadrilateral& other) const noexcept {
      return position == other.position && width == other.width &&
        height == other.height;
    }
    bool operator!=(const Quadrilateral& other) const noexcept {
      return !(*this == other);
    }

    SupaRL::Vector2D position;
    int width;
    int height;

    template <class Archive>
      void serialize(Archive& archive) {
        archive(position, width, height);
      }
  };

  template <typename T>
    inline T euclidean_squared(T x, T y) {
      return x * x + y * y;
    }
  inline int euclidean_squared(SupaRL::Vector2D vec) {
    return euclidean_squared(vec.x, vec.y);
  }

}

