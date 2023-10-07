#include "events/scroll_command.hpp"

namespace cpprl {

void ScrollCommand::execute() {
  // engine_.get_gui().scroll_current_view(scroll_amount_);
  engine_.scroll_current_view(scroll_amount_);
}
}  // namespace cpprl
