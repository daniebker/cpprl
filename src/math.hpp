#pragma once
#include <array>
namespace cpprl {
struct Vector2D {
  operator const std::array<int, 2>() const noexcept { return {x, y}; }

  Vector2D operator+(const Vector2D& other) const noexcept { return {x + other.x, y + other.y}; }
  Vector2D operator-(const Vector2D& other) const noexcept { return {x - other.x, y - other.y}; }
  bool operator==(const Vector2D& other) const noexcept { return x == other.x && y == other.y; }
  bool operator!=(const Vector2D& other) const noexcept { return !(*this == other); }

  int x;
  int y;
};
}  // namespace cpprl
