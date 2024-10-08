#pragma once
#include <array>
#include <cmath>

struct Vector2D {
  operator const std::array<int, 2>() const noexcept { return {x, y}; }

  Vector2D operator+(const Vector2D &other) const noexcept {
    return {x + other.x, y + other.y};
  }
  Vector2D operator+=(const Vector2D &other) noexcept {
    x += other.x;
    y += other.y;
    return *this;
  }
  Vector2D operator-(const Vector2D &other) const noexcept {
    return {x - other.x, y - other.y};
  }
  Vector2D operator/(int scalar) const noexcept {
    return {x / scalar, y / scalar};
  }
  bool operator==(const Vector2D &other) const noexcept {
    return x == other.x && y == other.y;
  }
  bool operator!=(const Vector2D &other) const noexcept {
    return !(*this == other);
  }
  float distance_to(const Vector2D &other) const noexcept {
    return std::sqrt(std::pow(x - other.x, 2) + std::pow(y - other.y, 2));
  }

  int x;
  int y;

  template <class Archive> void serialize(Archive &archive) { archive(x, y); }
};

inline double euclidean_distance(Vector2D vec, Vector2D other) {
  return pow(vec.x - other.x, 2) + pow(vec.y - other.y, 2);
}