#include "events/command.hpp"

#include <assert.h>

#include "combat_system.hpp"
#include "entity_manager.hpp"
#include "game_entity.hpp"
#include "gui.hpp"
#include "input_handler.hpp"
#include "state.hpp"
#include "world.hpp"
#include "core/coordinator.hpp"
#include "components/velocity.hpp"

extern SupaRL::Coordinator g_coordinator;
namespace cpprl {

  StateResult PickupCommand::execute() {
    std::optional<std::reference_wrapper<Entity>> optional_item_ref =
      world_.get_entities().get_non_blocking_entity_at(
          entity_->get_transform_component().position_);
    if(!optional_item_ref.has_value()) {
      return NoOp{"There is nothing here to pick up."};
    }

    Entity& item = optional_item_ref.value().get();
    if (item.get_name().find("corpse") != std::string::npos) {
      return NoOp{"There is nothing here to pick up."};
    }

    world_.get_message_log().add_message(
        "You pick up the " + item.get_name() + ".", WHITE);
    entity_->get_container().add(&item);
    world_.get_entities().remove(&item);

    return EndTurn{};
  }

  StateResult DropItemCommand::execute() {
    if (item_index_ <= 1) {
      return NoOp{"You can't drop that."};
    }
    Entity* item = entity_->get_container().get_inventory()[item_index_ - 1];
    // TODO: this is why I can't drop a corpse
    // Should a corpse be consumable?
    ConsumableComponent* consumable_component = &item->get_consumable_component();
    if (!consumable_component) {
      return NoOp{"There's nothing to drop."};
    }
    ActionResult result = consumable_component->drop(item, entity_);
    world_.get_entities().spawn(item);
    if (std::holds_alternative<Failure>(result)) {
      std::string message = std::get<Failure>(result).message;
      return NoOp{message.c_str()};
    } else if (std::holds_alternative<Poll>(result)) {
      return Change{std::move(std::get<Poll>(result).new_state)};
    } else if (std::holds_alternative<Success>(result)) {
      return EndTurn{};
    }
    return {};
  }

  StateResult ScrollCommand::execute() {
    ui_window_.set_cursor(ui_window_.get_cursor() + scroll_amount_);
    return {};
  }

  StateResult ViewHistoryCommand::execute() {
    return Change{std::make_unique<ViewMessageHistoryState>(
        world_,
        new HistoryWindow(
          60, 35, {0, 0}, world_.get_message_log(), "Message Log"))};
  }

  StateResult InventoryCommand::execute() {
    return Change{std::make_unique<ViewInventoryState>(
        world_, new InventoryWindow(40, 20, {0, 0}, entity_, "Inventory"))};
  }

  // TODO: can this be a template?
  StateResult SelectItemCommand::execute() {
    int cursor = ui_window_.get_cursor();
    if (sub_command_ == ItemSubCommand::USE_ITEM) {
      auto use_item_command =
        std::make_unique<UseItemCommand>(world_, entity_, cursor);
      return use_item_command->execute();
    } else if (sub_command_ == ItemSubCommand::DROP_ITEM) {
      auto drop_item_command =
        std::make_unique<DropItemCommand>(world_, entity_, cursor);
      return drop_item_command->execute();
    }

    return {};
  }

  StateResult UseItemCommand::execute() {
    // TODO: this throws when inventory is empty
    Entity* item = entity_->get_container().get_inventory()[item_index_ - 1];
    ConsumableComponent* consumable_component = &item->get_consumable_component();
    if (!consumable_component) {
      return NoOp{"There's nothing to use."};
    }
    ActionResult result = consumable_component->use(item, entity_, world_);
    if (std::holds_alternative<Failure>(result)) {
      std::string message = std::get<Failure>(result).message;
      return NoOp{message};
    } else if (std::holds_alternative<Poll>(result)) {
      return Change{std::move(std::get<Poll>(result).new_state)};
    } else if (std::holds_alternative<Success>(result)) {
      return EndTurn{};
    }
    return {};
  }

  StateResult CloseViewCommand::execute() {
    return Change{std::make_unique<InGameState>(world_)};
  }

  StateResult DieEvent::execute() {
    world_.get_message_log().add_message(
        fmt::format("{} has died!", util::capitalize(entity_->get_name())));
    entity_->get_defense_component().die(*entity_);

    if (entity_->get_name() != "player") {
      const std::optional<std::reference_wrapper<StatsComponent>>
        stats_component = world_.get_player()->get_stats_component();
      assert(stats_component.has_value());
      world_.get_message_log().add_message(
          fmt::format(
            "You gain {} experience points.",
            entity_->get_stats_component().value().get().get_xp()),
          GREEN);
      stats_component.value().get().add_xp(
          entity_->get_stats_component().value().get().get_xp());
    }

    return {};
  }

  StateResult DirectionalCommand::execute() {
    auto targetPos =
      entity_->get_transform_component().position_ + move_vector_;

    if (world_.get_entities().get_blocking_entity_at(targetPos)) {
      auto action = MeleeCommand(world_, entity_, move_vector_);
      return action.execute();
    } else {
      auto action = MovementCommand(world_, entity_, move_vector_);
      return action.execute();
    }
  }

  StateResult MouseInputEvent::execute() {
    world_.get_map().set_highlight_tile(position_);
    return {};
  }

  StateResult MouseClickEvent::execute() {
    return Change{std::make_unique<InGameState>(world_)};
  }

  StateResult ExitTargetingModeCommand::execute() {
    return Change{std::make_unique<InGameState>(world_)};
  }

  StateResult MeleeCommand::execute() {
    auto targetPos =
      entity_->get_transform_component().position_ + move_vector_;
    std::optional<std::reference_wrapper<Entity>> target =
      world_.get_entities().get_blocking_entity_at(targetPos);

    tcod::ColorRGB attack_colour = WHITE;
    if (entity_->get_name() != "player") {
      attack_colour = RED;
    }

    if (target.has_value()) {
      int damage = combat_system::handle_attack(*entity_, target.value().get());
      if (damage > 0) {
        std::string message = fmt::format(
            "{} attacks {} for {} hit points.",
            util::capitalize(entity_->get_name()),
            util::capitalize(target.value().get().get_name()),
            damage);

        world_.get_message_log().add_message(message, attack_colour, true);

        if (target.value().get().get_defense_component().is_dead()) {
          auto action = DieEvent(world_, &target.value().get());
          return action.execute();
        }
      } else {
        std::string message = fmt::format(
            "{} attacks {} but does no damage.",
            util::capitalize(entity_->get_name()),
            util::capitalize(target.value().get().get_name()));

        world_.get_message_log().add_message(message, attack_colour, true);
      }
    }
    return EndTurn{};
  };

  StateResult MovementCommand::execute() {
    SupaRL::Vector2D new_position =
      entity_->get_transform_component().position_ + move_vector_;
    auto& map = world_.get_map();

    if (map.is_not_in_bounds(new_position)) {
      return NoOp{"You can't move out the bounds of space and time."};
    }

    if (world_.get_entities().get_blocking_entity_at(new_position)) {
      return EndTurn{};
    }

    if (map.is_walkable(new_position)) {

      entity_->get_transform_component().position_ = new_position;

      g_coordinator.get_component<SupaRL::VelocityComponent>(
          entity_->get_id()).velocity_ = {move_vector_.x, move_vector_.y};

    } else {
      return NoOp{"You can't walk on that."};
    }
    return EndTurn{};
  }

  StateResult UseCommand::execute() {
    auto tile_at = world_.get_map().get_tiles().at(position_);
    if (tile_at.type == TileType::down_stairs) {
      return Change{std::make_unique<NextLevelState>(world_)};
    } else {
      return NoOp{"There are no stairs here."};
    }
  }

  StateResult QuitCommand::execute() { return Quit{}; }

  StateResult SelectMenuItemCommand::execute() {
    int cursor = ui_window_.get_cursor();
    MenuSubCommand sub_command;

    auto subcommand_map = std::map<int, MenuSubCommand>{
      {1, MenuSubCommand::NEW_GAME},
        {2, MenuSubCommand::CONTINUE},
        {3, MenuSubCommand::QUIT},
    };

    sub_command = subcommand_map[cursor];

    if (sub_command && sub_command == MenuSubCommand::CONTINUE) {
      return LoadGame{};
    } else if (sub_command == MenuSubCommand::NEW_GAME) {
      auto reset_game_command = std::make_unique<ResetGameCommand>(world_);
      return reset_game_command->execute();
    } else if (sub_command == MenuSubCommand::QUIT) {
      return Quit{};
    }

    return {};
  }

  StateResult MainMenuCommand::execute() {
    return Change{std::make_unique<MainMenuState>(
        world_, new MainMenuWindow(60, 35, {0, 0}))};
  }

  StateResult CharacterMenuCommand::execute() {
    return Change{std::make_unique<CharacterMenuState>(
        world_, new CharacterMenuWindow(60, 35, {0, 0}, entity_))};
  }

  StateResult BoostStatCommand::execute() {
    int cursor = ui_window_.get_cursor();
    Entity* player = world_.get_player();
    auto& stats = player->get_stats_component().value().get();

    if (stats.get_stats_points() <= 0) {
      return NoOp{"You don't have any stat points."};
    }

    if (cursor == 3) {
      player->get_attack_component().boost_damage(1);
    } else if (cursor == 4) {
      player->get_defense_component().boost_defense(1);
    }
    stats.reduce_stats_points(1);
    return EndTurn{};
  }
}
