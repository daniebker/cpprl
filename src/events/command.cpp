#include "events/command.hpp"

#include "combat_system.hpp"
#include "entity_manager.hpp"
#include "exceptions.hpp"
#include "game_entity.hpp"
#include "gui.hpp"
#include "input_handler.hpp"
#include "state.hpp"
#include "world.hpp"

namespace cpprl {

StateResult PickupCommand::execute() {
  Entity* item = world_.get_entities().get_non_blocking_entity_at(
      entity_->get_transform_component()->get_position());
  if (item) {
    world_.get_message_log().add_message(
        "You pick up the " + item->get_name() + ".", WHITE);
    entity_->get_container()->add(item);
    world_.get_entities().remove(item);
  } else {
    throw Impossible("There is nothing here to pick up.");
  }

  return EndTurn{};
}

StateResult ScrollCommand::execute() {
  ui_window_.set_cursor(ui_window_.get_cursor() + scroll_amount_);
  // world_.scroll_current_view(scroll_amount_);
  return {};
}

StateResult ViewHistoryCommand::execute() {
  // world_.toggle_pause();
  // world_.toggle_view();
  // world_.set_current_view();

  // world_.set_input_handler(new HistoryViewInputHandler(world_));
  return Change{std::make_unique<ViewMessageHistoryState>(
      world_,
      new HistoryWindow(
          80, 40, {0, 0}, world_.get_message_log(), "Message Log"))};
}

StateResult InventoryCommand::execute() {
  return Change{std::make_unique<ViewInventoryState>(
      world_, new InventoryWindow(40, 20, {0, 0}, entity_, "Inventory"))};
}

StateResult SelectItemCommand::execute() {
  int cursor_ = ui_window_.get_cursor();
  auto use_item_command =
      std::make_unique<UseItemCommand>(world_, entity_, cursor_);
  return use_item_command->execute();
}

StateResult UseItemCommand::execute() {
  Entity* item = entity_->get_container()->get_inventory()[item_index_ - 1];
  ConsumableComponent* consumable_component = item->get_consumable_component();
  if (!consumable_component) {
    throw Impossible("There's nothing to use.");
  }
  ActionResult result = consumable_component->use(item, entity_, world_);
  if (std::holds_alternative<Failure>(result)) {
    std::string message = std::get<Failure>(result).message;
    throw Impossible(message.c_str());
  } else if (std::holds_alternative<Poll>(result)) {
    return Change{std::move(std::get<Poll>(result).new_state)};
  } else if (std::holds_alternative<Success>(result)) {
    return EndTurn{};
  }
  return {};
}

StateResult CloseViewCommand::execute() {
  world_.toggle_pause();
  world_.toggle_view();
  // world_.set_input_handler(new GameInputHandler(world_,
  // world_.get_player()));
  return Change{std::make_unique<InGameState>(world_)};
}

StateResult DieEvent::execute() {
  if (entity_->get_name() == "player") {
    world_.handle_player_death();
    // TODO: return GameOver{}
  }
  world_.get_message_log().add_message(
      fmt::format("{} has died!", util::capitalize(entity_->get_name())));
  entity_->get_defense_component()->die(*entity_);
  return {};
}

StateResult DirectionalCommand::execute() {
  auto targetPos =
      entity_->get_transform_component()->get_position() + move_vector_;

  if (world_.get_entities().get_blocking_entity_at(targetPos)) {
    auto action = MeleeCommand(world_, entity_, move_vector_);
    return action.execute();
  } else {
    auto action = MovementCommand(world_, entity_, move_vector_);
    return action.execute();
  }
}

StateResult MouseInputEvent::execute() {
  // world_.get_controller().cursor = position_;
  world_.get_map().set_highlight_tile(position_);
  return {};
}

StateResult MouseClickEvent::execute() {
  // world_.get_controller().cursor = position_;
  // world_.get_map()->set_highlight_tile(position_);
  bool success =
      world_.get_map().set_target_tile(position_, *world_.get_player());
  if (success) {
    // world_.toggle_targeting_mode();
  }
  return {};
}

StateResult ExitTargetingModeCommand::execute() {
  return Change{std::make_unique<InGameState>(world_)};
}

StateResult MeleeCommand::execute() {
  auto targetPos =
      entity_->get_transform_component()->get_position() + move_vector_;
  Entity* target = world_.get_entities().get_blocking_entity_at(targetPos);

  tcod::ColorRGB attack_colour = WHITE;
  if (entity_->get_name() != "player") {
    attack_colour = RED;
  }

  if (target) {
    int damage = combat_system::handle_attack(*entity_, *target);
    if (damage > 0) {
      std::string message = fmt::format(
          "{} attacks {} for {} hit points.",
          util::capitalize(entity_->get_name()),
          util::capitalize(target->get_name()),
          damage);

      world_.get_message_log().add_message(message, attack_colour, true);

      if (target->get_defense_component()->is_dead()) {
        auto action = DieEvent(world_, target);
        action.execute();
      }
    } else {
      std::string message = fmt::format(
          "{} attacks {} but does no damage.",
          util::capitalize(entity_->get_name()),
          util::capitalize(target->get_name()));

      world_.get_message_log().add_message(message, attack_colour, true);
    }
  }
  return EndTurn{};
};

StateResult MovementCommand::execute() {
  Vector2D new_position =
      entity_->get_transform_component()->get_position() + move_vector_;
  auto& map = world_.get_map();

  if (map.is_not_in_bounds(new_position)) {
    throw Impossible("You can't move out the bounds of space and time.");
  }

  if (world_.get_entities().get_blocking_entity_at(new_position)) {
    return EndTurn{};
  }

  if (map.is_walkable(new_position)) {
    entity_->get_transform_component()->move(new_position);
  } else {
    throw Impossible("You can't walk on that.");
  }
  return EndTurn{};
}

StateResult QuitCommand::execute() { return Quit{}; }
}  // namespace cpprl
