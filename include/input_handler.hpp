#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H
#include <SDL2/SDL.h>

#include <vector>

#include "events/close_view_command.hpp"
#include "events/directional_command.hpp"
#include "events/engine_event.hpp"
#include "events/quit_command.hpp"
#include "globals.hpp"

namespace cpprl {

class Engine;
class EngineEvent;

class EventHandler {
 public:
  EventHandler(Engine& engine)
      : engine_(engine), noop(engine), quitCommand_(engine){};
  virtual ~EventHandler() = default;
  virtual EngineEvent& handle_sdl_event(SDL_Event event) noexcept = 0;

 protected:
  Engine& engine_;
  NoOpEvent noop;

 private:
  QuitCommand quitCommand_;
};

class TargetingInputHandler final : public EventHandler {
 private:
  ExitTargetingModeCommand exit_targeting_mode_command_;

 public:
  TargetingInputHandler(Engine& engine)
      : EventHandler(engine), exit_targeting_mode_command_(engine){};
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
  GameInputHandler(Engine& engine, Entity* controllableEntity)
      : EventHandler(engine),
        buttonRight(engine_, controllableEntity, Vector2D{1, 0}),
        buttonUp(engine_, controllableEntity, Vector2D{0, -1}),
        buttonDown(engine_, controllableEntity, Vector2D{0, 1}),
        buttonUpRight(engine_, controllableEntity, Vector2D{1, -1}),
        buttonUpLeft(engine_, controllableEntity, Vector2D{-1, -1}),
        buttonLeft(engine_, controllableEntity, Vector2D{-1, 0}),
        buttonDownRight(engine_, controllableEntity, Vector2D{1, 1}),
        buttonDownLeft(engine_, controllableEntity, Vector2D{-1, 1}),
        viewHistoryCommand(engine_),
        pickupCommand_(engine, controllableEntity),
        inventoryCommand_(engine, controllableEntity){};

  virtual EngineEvent& handle_sdl_event(SDL_Event event) noexcept override;
};

class MenuInputHandler final : public EventHandler {
 private:
  ResetGameCommand resetGameCommand;

 public:
  MenuInputHandler(Engine& engine)
      : EventHandler(engine), resetGameCommand(engine){};
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
  GuiInputHandler(Engine& engine)
      : EventHandler(engine),
        closeViewCommand_(engine),
        scrollDownCommand_(engine, 1),
        scrollUpCommand_(engine, -1),
        jumpUpCommand_(engine, -10),
        jumpDownCommand_(engine, 10),
        jumpToHome_(engine, 0){};

  virtual EngineEvent& handle_sdl_event(SDL_Event event) noexcept override;
};

class HistoryViewInputHandler final : public GuiInputHandler {
 public:
  HistoryViewInputHandler(Engine& engine) : GuiInputHandler(engine){};

  virtual EngineEvent& handle_sdl_event(SDL_Event event) noexcept override;
};

class InventoryInputHandler final : public GuiInputHandler {
 private:
  SelectItemCommand selectItemCommand_;

 public:
  InventoryInputHandler(Engine& engine, Entity* entity)
      : GuiInputHandler(engine), selectItemCommand_(engine, entity){};
  EngineEvent& handle_sdl_event(SDL_Event event) noexcept override;
};

}  // namespace cpprl
#endif
