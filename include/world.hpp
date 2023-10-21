#ifndef WORLD_HPP
#define WORLD_HPP

#include "entity_manager.hpp"
#include "game_entity.hpp"

namespace cpprl {
class World {
 private:
  std::unique_ptr<EntityManager> entities_;
  std::shared_ptr<Entity> player_;
  std::unique_ptr<Dungeon> dungeon_;
  std::unique_ptr<Map> map_;
  std::unique_ptr<MessageLog> message_log_;

 public:
  World()
      : entities_(nullptr),
        player_(nullptr),
        dungeon_(std::make_unique<Dungeon>(),
        map_(nullptr),
        message_log_(nullptr){};
  ~World();

  MessageLog& get_message_log() {
    return *message_log_; }
  Map& get_map() {
    return *map_; }
  EntityManager& get_entities() {
    return *entities_; }
  Entity& get_player() {
    return *player_; }

  void generate_map(int width, int height);
  void render();
  void handle_enemy_turns();
};
}  // namespace cpprl

#endif
