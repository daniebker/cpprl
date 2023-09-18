#include "command.hpp"

namespace cpprl {
class QuitCommand : public Command {
 public:
  QuitCommand();
  virtual ~QuitCommand();
  virtual void execute(GameEntity& entity);
};
}  // namespace cpprl
