#ifndef COMMAND_H
#define COMMAND_H

#include "game_entity.hpp"
#include "types/map.hpp"

namespace cpprl {
class Command {
 public:
  virtual void execute(Map& map, GameEntity& gameEntity) = 0;
};
}  // namespace cpprl
#endif
