#ifndef TYPES_MATH_HPP
#define TYPES_MATH_HPP

#include <array>

namespace cpprl {

struct Vector2D {
  operator const std::array<int, 2>() const noexcept { return {x, y}; }

  Vector2D operator+(const Vector2D& other) const noexcept { return {x + other.x, y + other.y}; }
  Vector2D operator+=(const Vector2D& other) noexcept {
    x += other.x;
    y += other.y;
    return *this;
  }
  Vector2D operator-(const Vector2D& other) const noexcept { return {x - other.x, y - other.y}; }
  Vector2D operator/(int scalar) const noexcept { return {x / scalar, y / scalar}; }
  bool operator==(const Vector2D& other) const noexcept { return x == other.x && y == other.y; }
  bool operator!=(const Vector2D& other) const noexcept { return !(*this == other); }

  int x;
  int y;
};

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

  Vector2D position;
  int width;
  int height;
};
}  // namespace cpprl

#endif  // TYPES_MATH_HPP
