#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H
#include <SDL2/SDL.h>

#include <vector>

#include "events/command.hpp"
#include "globals.hpp"
#include "types/world_fwd.hpp"

namespace cpprl {

class Entity;

class EventHandler {
 public:
  EventHandler(World& world)
      : world_(world), noop(world), quitCommand_(world){};
  virtual ~EventHandler() = default;
  virtual EngineEvent& handle_sdl_event(SDL_Event event) noexcept = 0;

 protected:
  World& world_;
  NoOpEvent noop;

 private:
  QuitCommand quitCommand_;
};

class TargetingInputHandler final : public EventHandler {
 private:
  ExitTargetingModeCommand exit_targeting_mode_command_;

 public:
  TargetingInputHandler(World& world)
      : EventHandler(world), exit_targeting_mode_command_(world){};
  virtual EngineEvent& handle_sdl_event(SDL_Event event) noexcept override;
};

class GameInputHandler final : public EventHandler {
 private:
  DirectionalCommand buttonRight;
  DirectionalCommand buttonUp;
  DirectionalCommand buttonDown;
  DirectionalCommand buttonUpRight;
  DirectionalCommand buttonUpLeft;
  DirectionalCommand buttonLeft;
  DirectionalCommand buttonDownRight;
  DirectionalCommand buttonDownLeft;
  ViewHistoryCommand viewHistoryCommand;
  PickupCommand pickupCommand_;
  InventoryCommand inventoryCommand_;

 public:
  GameInputHandler(World& world, Entity* controllable_entity);

  virtual EngineEvent& handle_sdl_event(SDL_Event event) noexcept override;
};

class MenuInputHandler final : public EventHandler {
 private:
  ResetGameCommand resetGameCommand;

 public:
  MenuInputHandler(World& world)
      : EventHandler(world), resetGameCommand(world){};
  virtual EngineEvent& handle_sdl_event(SDL_Event event) noexcept override;
};

class GuiInputHandler : public EventHandler {
 protected:
  CloseViewCommand closeViewCommand_;
  ScrollCommand scrollDownCommand_;
  ScrollCommand scrollUpCommand_;
  ScrollCommand jumpUpCommand_;
  ScrollCommand jumpDownCommand_;
  ScrollCommand jumpToHome_;

 public:
  GuiInputHandler(World& world);

  virtual EngineEvent& handle_sdl_event(SDL_Event event) noexcept override;
};

class HistoryViewInputHandler final : public GuiInputHandler {
 public:
  HistoryViewInputHandler(World& world) : GuiInputHandler(world){};

  virtual EngineEvent& handle_sdl_event(SDL_Event event) noexcept override;
};

class InventoryInputHandler final : public GuiInputHandler {
 private:
  SelectItemCommand selectItemCommand_;

 public:
  InventoryInputHandler(World& world, Entity* entity)
      : GuiInputHandler(world), selectItemCommand_(world, entity){};
  EngineEvent& handle_sdl_event(SDL_Event event) noexcept override;
};

}  // namespace cpprl
#endif
