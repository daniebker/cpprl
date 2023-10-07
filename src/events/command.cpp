#include "events/command.hpp"

#include "exceptions.hpp"
#include "input_handler.hpp"

namespace cpprl {

void PickupCommand::execute() {
  Entity* item = engine_.get_entities().get_non_blocking_entity_at(
      entity_->get_transform_component()->get_position());
  if (item) {
    engine_.get_message_log().add_message(
        "You pick up the " + item->get_name() + ".", WHITE);
    entity_->get_container()->add(item);
    engine_.get_entities().remove(item);
  } else {
    throw Impossible("There is nothing here to pick up.");
  }
}

void ScrollCommand::execute() { engine_.scroll_current_view(scroll_amount_); }

void ViewHistoryCommand::execute() {
  engine_.toggle_pause();
  engine_.toggle_view();
  engine_.set_current_view(new HistoryWindow(
      80, 40, {0, 0}, engine_.get_message_log(), "Message Log"));
  engine_.set_input_handler(new HistoryViewInputHandler(engine_));
}

void InventoryCommand::execute() {
  engine_.toggle_pause();
  engine_.toggle_view();
  engine_.set_current_view(
      new InventoryWindow(40, 20, {0, 0}, entity_, "Inventory"));
  engine_.set_input_handler(new InventoryInputHandler(engine_, entity_));
}

void SelectItemCommand::execute() {
  int cursor_ = engine_.get_current_view().get_cursor();
  auto use_item_command =
      std::make_unique<UseItemCommand>(engine_, entity_, cursor_);
  use_item_command->execute();
}

void UseItemCommand::execute() {
  Entity* item = entity_->get_container()->get_inventory()[item_index_ - 1];
  ConsumableComponent* consumable_component = item->get_consumable_component();
  if (!consumable_component) {
    throw Impossible("There's nothing to use.");
  }
  try {
    consumable_component->use(item, entity_);
  } catch (Impossible& e) {
    engine_.get_message_log().add_message(e.what(), WHITE);
  }
}
}  // namespace cpprl
