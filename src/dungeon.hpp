#pragma once

#include <vector>

#include "types/map.hpp"

namespace cpprl {
class Dungeon {
 private:
  Map map_;
  std::vector<Vector2D> l_tunnel_between(Vector2D start, Vector2D end);

 public:
  Dungeon() : map_(Map(0, 0)){};
  ~Dungeon(){};
  Map generate(int width, int height);
};
}  // namespace cpprl
