#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H
#include <SDL2/SDL.h>

#include <vector>

#include "events/directional_command.hpp"
#include "events/engine_event.hpp"
#include "events/quit_command.hpp"

namespace cpprl {

class Engine;
class EngineEvent;

class InputHandler {
 public:
  InputHandler(Engine& engine) : engine_(engine), quitCommand(engine), noop(engine){};
  virtual EngineEvent& handle_input(SDL_Keycode key);

 protected:
  Engine& engine_;

 private:
  QuitCommand quitCommand;
  NoOpEvent noop;
};

class GameInputHandler : public InputHandler {
 private:
  DirectionalCommand buttonRight;
  DirectionalCommand buttonUp;
  DirectionalCommand buttonDown;
  DirectionalCommand buttonUpRight;
  DirectionalCommand buttonUpLeft;
  DirectionalCommand buttonLeft;
  DirectionalCommand buttonDownRight;
  DirectionalCommand buttonDownLeft;

 public:
  GameInputHandler(Engine& engine, GameEntity& controllableEntity)
      : InputHandler(engine),
        buttonRight(engine_, controllableEntity, Vector2D{1, 0}),
        buttonUp(engine_, controllableEntity, Vector2D{0, -1}),
        buttonDown(engine_, controllableEntity, Vector2D{0, 1}),
        buttonUpRight(engine_, controllableEntity, Vector2D{1, -1}),
        buttonUpLeft(engine_, controllableEntity, Vector2D{-1, -1}),
        buttonLeft(engine_, controllableEntity, Vector2D{-1, 0}),
        buttonDownRight(engine_, controllableEntity, Vector2D{1, 1}),
        buttonDownLeft(engine_, controllableEntity, Vector2D{-1, 1}){};

  virtual EngineEvent& handle_input(SDL_Keycode key) override;
};

class MenuInputHandler : public InputHandler {
 private:
  ResetGameCommand resetGameCommand;

 public:
  MenuInputHandler(Engine& engine) : InputHandler(engine), resetGameCommand(engine){};
  virtual EngineEvent& handle_input(SDL_Keycode key) override;
};

}  // namespace cpprl
#endif
