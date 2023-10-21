#include "../../include/events/quit_command.hpp"

#include "iostream"

namespace cpprl {
CommandResult QuitCommand::execute() { return Quit{}; }
}  // namespace cpprl
