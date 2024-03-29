#ifndef WORLD_HPP
#define WORLD_HPP

#include <libtcod.hpp>

#include "dungeon.hpp"
#include "entity_factory.hpp"
#include "message_log.hpp"
#include "rendering.hpp"
#include "types/map.hpp"
#include "ui/ui.hpp"

namespace cpprl {
class EntityManager;
class Entity;
class UiWindow;
struct Controller;

class World {
 private:
  std::unique_ptr<EntityManager> entities_;
  Dungeon dungeon_;
  MessageLog message_log_;
  std::unique_ptr<UI> ui_;
  std::unique_ptr<Controller> controller_;
  UiWindow* current_window_;
  Entity* player_;

 public:
  World();
  virtual ~World() = default;

  MessageLog& get_message_log() { return message_log_; }
  Map& get_map() { return dungeon_.get_map(); }
  EntityManager& get_entities() { return *entities_; }
  void reset();

  void generate_map(int width, int height, bool with_entities = false);
  Dungeon& get_dungeon() { return dungeon_; }
  void render(Renderer& renderer);
  void handle_enemy_turns();
  void scroll_current_view(int scroll_amount);
  void handle_player_death();
  void set_targeting_tile(
      float max_range = 0.0f, std::function<void()> callback = nullptr);
  Entity* get_player() const { return player_; }
  void spawn_player();
  void spawn_player(Entity* player);

  template <class Archive>
  void save(Archive& archive) const {
    archive(dungeon_);
    int width = dungeon_.get_map().get_width();
    int height = dungeon_.get_map().get_height();
    archive(width, height);
    for (int y{0}; y < dungeon_.get_map().get_height(); ++y) {
      for (int x{0}; x < dungeon_.get_map().get_width(); ++x) {
        archive(dungeon_.get_map().get_tiles().at({x, y}).explored);
      }
    }
    archive(entities_);
    player_->pack(archive);
    archive(message_log_);
  }

  template <class Archive>
  void load(Archive& archive) {
    archive(dungeon_);
    int width;
    int height;
    archive(width, height);
    generate_map(width, height, false);
    for (int y{0}; y < dungeon_.get_map().get_height(); ++y) {
      for (int x{0}; x < dungeon_.get_map().get_width(); ++x) {
        archive(dungeon_.get_map().get_tiles().at({x, y}).explored);
      }
    }
    archive(entities_);
    player_ = new Entity("", false, nullptr, nullptr);
    // TODO: If player is confused, quitting and reopening the game removes the
    // confused state
    player_->unpack(archive);
    spawn_player(player_);
    archive(message_log_);
  }
};
}  // namespace cpprl

#endif
