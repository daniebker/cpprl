#ifndef ENGINE_HPP
#define ENGINE_HPP
#include <libtcod.hpp>
#include <list>

#include "input_handler.hpp"

namespace cpprl {
class Engine {
 private:
  std::list<GameEntity*> entities_;
  InputHandler input_handler_;

 public:
  Engine(std::list<GameEntity*> entities, InputHandler input_handler);
  virtual ~Engine();
  void handle_events();
  void render(TCOD_Console& console);
};
}  // namespace cpprl
#endif
