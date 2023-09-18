#ifndef COMMAND_H
#define COMMAND_H

#include "game_entity.hpp"

namespace cpprl {
class Command {
 public:
  virtual void execute(GameEntity& gameEntity) = 0;
};
}  // namespace cpprl
#endif
