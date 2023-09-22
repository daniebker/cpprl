#pragma once

#include "types/map.hpp"

namespace cpprl {
class Dungeon {
 private:
  Map map_;

 public:
  Dungeon() : map_(Map(0, 0)){};
  ~Dungeon(){};
  Map generate(int width, int height);
};
}  // namespace cpprl
