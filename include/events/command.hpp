#ifndef COMMAND_H
#define COMMAND_H

#include "types/entity_fwd.hpp"
#include "types/math.hpp"
#include "types/state_result.hpp"
#include "types/world_fwd.hpp"

namespace cpprl {
class UiWindow;

class EngineEvent {
 protected:
  World& world_;

 public:
  EngineEvent(World& world) : world_(world) {}
  virtual ~EngineEvent() {}
  virtual StateResult execute() = 0;
};

class Command : public EngineEvent {
 protected:
  Entity* entity_;

 public:
  Command(World& world, Entity* entity) : EngineEvent(world), entity_(entity) {}
  virtual ~Command() {}
  virtual StateResult execute() = 0;
};

class ScrollCommand : public EngineEvent {
 public:
  ScrollCommand(World& world, UiWindow& ui_window, int scroll_amount)
      : EngineEvent(world),
        ui_window_(ui_window),
        scroll_amount_(scroll_amount){};
  virtual StateResult execute();

 private:
  UiWindow& ui_window_;
  int scroll_amount_;
};

class ViewHistoryCommand : public EngineEvent {
 public:
  ViewHistoryCommand(World& world) : EngineEvent(world){};
  virtual StateResult execute();
};

class PickupCommand : public Command {
 public:
  PickupCommand(World& world, Entity* entity) : Command(world, entity){};
  StateResult execute() override;
};

// TODO: base class from useItemCommand
class DropItemCommand final : public Command {
 private:
  int item_index_;

 public:
  DropItemCommand(World& world, Entity* entity, int item_index)
      : Command(world, entity), item_index_(item_index){};
  ~DropItemCommand() override = default;
  StateResult execute() override;
};

class InventoryCommand final : public Command {
 public:
  InventoryCommand(World& world, Entity* entity) : Command(world, entity) {}
  StateResult execute() override;
};

class MainMenuCommand final : public EngineEvent {
 public:
  MainMenuCommand(World& world) : EngineEvent(world) {}
  StateResult execute() override;
};

enum ItemSubCommand { USE_ITEM, DROP_ITEM };
class SelectItemCommand final : public Command {
 private:
  ItemSubCommand sub_command_;
  UiWindow& ui_window_;

 public:
  SelectItemCommand(
      World& world,
      Entity* entity,
      UiWindow& ui_window,
      ItemSubCommand sub_command)
      : Command(world, entity),
        sub_command_(sub_command),
        ui_window_(ui_window) {}
  StateResult execute() override;
};

enum MenuSubCommand { NEW_GAME, CONTINUE, QUIT };
class SelectMenuItemCommand final : public EngineEvent {
 private:
  UiWindow& ui_window_;

 public:
  SelectMenuItemCommand(World& world, UiWindow& ui_window)
      : EngineEvent(world), ui_window_(ui_window) {}
  StateResult execute() override;
};

class UseItemCommand final : public Command {
 private:
  int item_index_;

 public:
  UseItemCommand(World& world, Entity* entity, int item_index)
      : Command(world, entity), item_index_(item_index) {}
  StateResult execute() override;
};

class CloseViewCommand final : public EngineEvent {
 public:
  CloseViewCommand(World& world) : EngineEvent(world){};
  virtual StateResult execute() override;
};

class DieEvent : public EngineEvent {
 public:
  DieEvent(World& world, Entity* entity)
      : EngineEvent(world), entity_(entity) {}
  virtual ~DieEvent() = default;
  virtual StateResult execute() override;

 private:
  Entity* entity_;
};

class DirectionalCommand : public Command {
 protected:
  Vector2D move_vector_;

 public:
  DirectionalCommand(World& world, Entity* entity, Vector2D move_vector)
      : Command(world, entity), move_vector_(move_vector){};
  virtual StateResult execute();
};

class NoOpEvent : public EngineEvent {
 public:
  NoOpEvent(World& world) : EngineEvent(world) {}
  StateResult execute() override { return {}; }
};

class ResetGameCommand : public EngineEvent {
 public:
  ResetGameCommand(World& world) : EngineEvent(world) {}
  StateResult execute() override { return Reset{}; }
};

class MouseInputEvent final : public EngineEvent {
 private:
  Vector2D position_;

 public:
  MouseInputEvent(World& world, Vector2D position)
      : EngineEvent(world), position_(position) {}
  StateResult execute() override;
};

class MouseClickEvent final : public EngineEvent {
  // TODO: remove this?
 private:
  Vector2D position_;

 public:
  MouseClickEvent(World& world, Vector2D position)
      : EngineEvent(world), position_(position) {}
  StateResult execute() override;
};

class ExitTargetingModeCommand final : public EngineEvent {
 public:
  ExitTargetingModeCommand(World& world) : EngineEvent(world) {}
  StateResult execute() override;
};

class MeleeCommand : DirectionalCommand {
 public:
  MeleeCommand(World& world, Entity* entity, Vector2D target_vector)
      : DirectionalCommand(world, entity, target_vector){};
  StateResult execute();
};

class MovementCommand : public DirectionalCommand {
 public:
  MovementCommand(World& world, Entity* entity, Vector2D move_vector)
      : DirectionalCommand(world, entity, move_vector){};
  virtual StateResult execute();
};
class QuitCommand : public EngineEvent {
 public:
  QuitCommand(World& world) : EngineEvent(world){};
  virtual StateResult execute();
};
}  // namespace cpprl
#endif
