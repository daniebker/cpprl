#ifndef WORLD_HPP
#define WORLD_HPP

#include <libtcod.hpp>

#include "message_log.hpp"
#include "persistent.hpp"
#include "rendering.hpp"
#include "types/map.hpp"

namespace cpprl {
class Dungeon;
class EntityManager;
class Entity;
class UiWindow;
struct Controller;

class World : public Persistent {
 private:
  std::unique_ptr<EntityManager> entities_;
  Entity* player_;
  std::unique_ptr<Dungeon> dungeon_;
  Map* map_;
  std::unique_ptr<MessageLog> message_log_;
  UiWindow* health_bar_;
  std::unique_ptr<Controller> controller_;
  UiWindow* current_window_;

 public:
  World();
  ~World() = default;

  MessageLog& get_message_log() { return *message_log_; }
  Map& get_map() { return *map_; }
  EntityManager& get_entities() { return *entities_; }
  Entity* get_player() { return player_; }
  void reset();

  void save(TCODZip& zip) override;
  void load(TCODZip& zip) override;
  void generate_map(int width, int height, bool with_entities = false);
  void render(Renderer& renderer);
  void handle_enemy_turns();
  void scroll_current_view(int scroll_amount);
  void handle_player_death();
  void set_targeting_tile(
      float max_range = 0.0f, std::function<void()> callback = nullptr);
};
}  // namespace cpprl

#endif
