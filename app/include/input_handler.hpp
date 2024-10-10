#pragma once

#include <SDL2/SDL.h>


#include "events/command.hpp"
#include "gui.hpp"
#include "types/world_fwd.hpp"

namespace cpprl {

  class Entity;

  class EventHandler {
    public:
      EventHandler(World& world)
        : world_(world),
        noop_(std::make_unique<NoOpEvent>(world)),
        quitCommand_(std::make_unique<QuitCommand>(world)){};
      virtual ~EventHandler() = default;
      virtual EngineEvent& handle_sdl_event(SDL_Event event) noexcept = 0;

    protected:
      World& world_;
      std::unique_ptr<NoOpEvent> noop_;

    private:
      std::unique_ptr<QuitCommand> quitCommand_;
  };

  class TargetingInputHandler final : public EventHandler {
    private:
      std::unique_ptr<ExitTargetingModeCommand> exit_targeting_mode_command_;
      std::unique_ptr<MouseInputEvent> mouse_input_event_;
      std::unique_ptr<MouseClickEvent> mouse_click_event_;

    public:
      TargetingInputHandler(World& world)
        : EventHandler(world),
        exit_targeting_mode_command_(
            std::make_unique<ExitTargetingModeCommand>(world)){};
      ~TargetingInputHandler(){};
      EngineEvent& handle_sdl_event(SDL_Event event) noexcept override;
  };

  class GameInputHandler final : public EventHandler {
    private:
      std::unique_ptr<DirectionalCommand> button_right_;
      std::unique_ptr<DirectionalCommand> button_up_;
      std::unique_ptr<DirectionalCommand> button_down_;
      std::unique_ptr<DirectionalCommand> button_up_right_;
      std::unique_ptr<DirectionalCommand> button_up_left_;
      std::unique_ptr<DirectionalCommand> button_left_;
      std::unique_ptr<DirectionalCommand> button_down_right_;
      std::unique_ptr<DirectionalCommand> button_down_left_;
      std::unique_ptr<ViewHistoryCommand> view_history_command_;
      std::unique_ptr<PickupCommand> pick_up_command_;
      std::unique_ptr<InventoryCommand> inventory_command_;
      std::unique_ptr<MainMenuCommand> main_menu_command_;
      std::unique_ptr<UseCommand> use_command_;
      std::unique_ptr<CharacterMenuCommand> character_menu_command_;
      Entity* controllable_entity_;

    public:
      GameInputHandler(World& world, Entity* controllable_entity);
      ~GameInputHandler() = default;

      EngineEvent& handle_sdl_event(SDL_Event event) noexcept override;
  };

  class MenuInputHandler final : public EventHandler {
    private:
      std::unique_ptr<ResetGameCommand> resetGameCommand_;

    public:
      MenuInputHandler(World& world)
        : EventHandler(world),
        resetGameCommand_(std::make_unique<ResetGameCommand>(world)){};
      ~MenuInputHandler() = default;
      EngineEvent& handle_sdl_event(SDL_Event event) noexcept override;
  };

  class GuiInputHandler : public EventHandler {
    protected:
      std::unique_ptr<CloseViewCommand> closeViewCommand_;
      std::unique_ptr<ScrollCommand> scrollDownCommand_;
      std::unique_ptr<ScrollCommand> scrollUpCommand_;
      std::unique_ptr<ScrollCommand> jumpUpCommand_;
      std::unique_ptr<ScrollCommand> jumpDownCommand_;
      std::unique_ptr<ScrollCommand> jumpToHome_;

    public:
      GuiInputHandler(World& world, UiWindow& ui_window);
      ~GuiInputHandler() = default;

      EngineEvent& handle_sdl_event(SDL_Event event) noexcept override;
  };

  class HistoryViewInputHandler final : public GuiInputHandler {
    public:
      HistoryViewInputHandler(World& world, UiWindow& ui_window)
        : GuiInputHandler(world, ui_window){};
      ~HistoryViewInputHandler() = default;

      EngineEvent& handle_sdl_event(SDL_Event event) noexcept override;
  };

  class InventoryInputHandler final : public GuiInputHandler {
    private:
      std::unique_ptr<SelectItemCommand> selectItemCommand_;
      std::unique_ptr<SelectItemCommand> dropItemCommand_;

    public:
      InventoryInputHandler(World& world, Entity* entity, UiWindow& ui_window)
        : GuiInputHandler(world, ui_window),
        selectItemCommand_(std::make_unique<SelectItemCommand>(
              world, entity, ui_window, ItemSubCommand::USE_ITEM)),
        dropItemCommand_(std::make_unique<SelectItemCommand>(
              world, entity, ui_window, ItemSubCommand::DROP_ITEM)){};
      ~InventoryInputHandler() = default;
      EngineEvent& handle_sdl_event(SDL_Event event) noexcept override;
  };

  class CharacterMenuInputHandler final : public GuiInputHandler {
    private:
      std::unique_ptr<BoostStatCommand> boost_stat_command_;

    public:
      CharacterMenuInputHandler(World& world, UiWindow& ui_window)
        : GuiInputHandler(world, ui_window),
        boost_stat_command_(
            std::make_unique<BoostStatCommand>(world, ui_window)){};
      EngineEvent& handle_sdl_event(SDL_Event event) noexcept override;
  };

  class MainMenuInputHandler final : public GuiInputHandler {
    private:
      std::unique_ptr<SelectMenuItemCommand> selectMenuItemCommand_;

    public:
      MainMenuInputHandler(World& world, UiWindow& ui_window)
        : GuiInputHandler(world, ui_window),
        selectMenuItemCommand_(
            std::make_unique<SelectMenuItemCommand>(world, ui_window)){};
      virtual ~MainMenuInputHandler() = default;
      EngineEvent& handle_sdl_event(SDL_Event event) noexcept override;
  };
}
