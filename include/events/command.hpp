#ifndef COMMAND_H
#define COMMAND_H

#include "engine.hpp"
#include "events/engine_event.hpp"
#include "game_entity.hpp"
#include "types/map.hpp"
#include "types/state_result.hpp"

namespace cpprl {
class Command : public EngineEvent {
 protected:
  Entity* entity_;

 public:
  Command(Engine& engine, Entity* entity)
      : EngineEvent(engine), entity_(entity) {}
  virtual ~Command() {}
  virtual CommandResult execute() = 0;
};

class ScrollCommand : public EngineEvent {
 public:
  ScrollCommand(Engine& engine, int scroll_amount)
      : EngineEvent(engine), scroll_amount_(scroll_amount){};
  virtual CommandResult execute();

 private:
  int scroll_amount_;
};

class ViewHistoryCommand : public EngineEvent {
 public:
  ViewHistoryCommand(Engine& engine) : EngineEvent(engine){};
  virtual CommandResult execute();
};

class PickupCommand : public Command {
 public:
  PickupCommand(Engine& engine, Entity* entity) : Command(engine, entity){};
  virtual CommandResult execute();
};

class InventoryCommand final : public Command {
 public:
  InventoryCommand(Engine& engine, Entity* entity) : Command(engine, entity) {}
  CommandResult execute() override;
};

class SelectItemCommand final : public Command {
 public:
  SelectItemCommand(Engine& engine, Entity* entity) : Command(engine, entity) {}
  CommandResult execute() override;
};

class UseItemCommand final : public Command {
 private:
  int item_index_;

 public:
  UseItemCommand(Engine& engine, Entity* entity, int item_index)
      : Command(engine, entity), item_index_(item_index) {}
  CommandResult execute() override;
};
}  // namespace cpprl
#endif
