#include "quit_command.hpp"

#include "iostream"

namespace cpprl {
QuitCommand::QuitCommand() {}
QuitCommand::~QuitCommand() {}
void QuitCommand::execute(Map* map, GameEntity* entity) { std::exit(0); }
}  // namespace cpprl
