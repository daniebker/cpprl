#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H
#include <SDL2/SDL.h>

#include <vector>

#include "events/command.hpp"
#include "globals.hpp"
#include "gui.hpp"
#include "types/world_fwd.hpp"

namespace cpprl {

class Entity;

class EventHandler {
 public:
  EventHandler(World& world)
      : world_(world),
        noop_(new NoOpEvent(world)),
        quitCommand_(new QuitCommand(world)){};
  virtual ~EventHandler() = default;
  virtual EngineEvent* handle_sdl_event(SDL_Event event) noexcept = 0;

 protected:
  World& world_;
  NoOpEvent* noop_;

 private:
  QuitCommand* quitCommand_;
};

class TargetingInputHandler final : public EventHandler {
 private:
  ExitTargetingModeCommand* exit_targeting_mode_command_;

 public:
  TargetingInputHandler(World& world)
      : EventHandler(world),
        exit_targeting_mode_command_(new ExitTargetingModeCommand(world)){};
  ~TargetingInputHandler(){};
  virtual EngineEvent* handle_sdl_event(SDL_Event event) noexcept override;
};

class GameInputHandler final : public EventHandler {
 private:
  DirectionalCommand* button_right_;
  DirectionalCommand* button_up_;
  DirectionalCommand* button_down_;
  DirectionalCommand* button_up_right_;
  DirectionalCommand* button_up_left_;
  DirectionalCommand* button_left_;
  DirectionalCommand* button_down_right_;
  DirectionalCommand* button_down_left_;
  ViewHistoryCommand* view_history_command_;
  PickupCommand* pick_up_command_;
  InventoryCommand* inventory_command_;
  MainMenuCommand* main_menu_command_;

 public:
  GameInputHandler(World& world, Entity* controllable_entity);
  ~GameInputHandler() = default;

  virtual EngineEvent* handle_sdl_event(SDL_Event event) noexcept override;
};

class MenuInputHandler final : public EventHandler {
 private:
  ResetGameCommand* resetGameCommand_;

 public:
  MenuInputHandler(World& world)
      : EventHandler(world), resetGameCommand_(new ResetGameCommand(world)){};
  ~MenuInputHandler() = default;
  virtual EngineEvent* handle_sdl_event(SDL_Event event) noexcept override;
};

class GuiInputHandler : public EventHandler {
 protected:
  CloseViewCommand* closeViewCommand_;
  ScrollCommand* scrollDownCommand_;
  ScrollCommand* scrollUpCommand_;
  ScrollCommand* jumpUpCommand_;
  ScrollCommand* jumpDownCommand_;
  ScrollCommand* jumpToHome_;

 public:
  GuiInputHandler(World& world, UiWindow& ui_window);
  ~GuiInputHandler() = default;

  virtual EngineEvent* handle_sdl_event(SDL_Event event) noexcept override;
};

class HistoryViewInputHandler final : public GuiInputHandler {
 public:
  HistoryViewInputHandler(World& world, UiWindow& ui_window)
      : GuiInputHandler(world, ui_window){};
  ~HistoryViewInputHandler() = default;

  virtual EngineEvent* handle_sdl_event(SDL_Event event) noexcept override;
};

class InventoryInputHandler final : public GuiInputHandler {
 private:
  SelectItemCommand* selectItemCommand_;
  SelectItemCommand* dropItemCommand_;

 public:
  InventoryInputHandler(World& world, Entity* entity, UiWindow& ui_window)
      : GuiInputHandler(world, ui_window),
        selectItemCommand_(new SelectItemCommand(
            world, entity, ui_window, ItemSubCommand::USE_ITEM)),
        dropItemCommand_(new SelectItemCommand(
            world, entity, ui_window, ItemSubCommand::DROP_ITEM)){};
  ~InventoryInputHandler() = default;
  EngineEvent* handle_sdl_event(SDL_Event event) noexcept override;
};

class MainMenuInputHandler final : public GuiInputHandler {
 private:
  SelectMenuItemCommand* selectMenuItemCommand_;

 public:
  MainMenuInputHandler(World& world, UiWindow& ui_window)
      : GuiInputHandler(world, ui_window),
        selectMenuItemCommand_(new SelectMenuItemCommand(world, ui_window)){};
  ~MainMenuInputHandler() = default;
  EngineEvent* handle_sdl_event(SDL_Event event) noexcept override;
};
}  // namespace cpprl
#endif
