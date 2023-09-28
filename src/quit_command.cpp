#include "quit_command.hpp"

#include "iostream"

namespace cpprl {
void QuitCommand::execute() { std::exit(0); }
}  // namespace cpprl
