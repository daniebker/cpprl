#include "dungeon.hpp"

#include <tuple>

#include "rectangular_room.hpp"
#include "types/map.hpp"

namespace cpprl {
Map* Dungeon::generate(DungeonConfig dungeon_config, GameEntity& player) {
  auto map = new Map(dungeon_config.map_width, dungeon_config.map_height, {player});
  auto rooms = std::vector<RectangularRoom>{};

  auto* random = TCODRandom::getInstance();
  for (int i = 0; i < dungeon_config.max_rooms; i++) {
    int room_width = random->getInt(dungeon_config.room_min_size, dungeon_config.room_max_size);
    int room_height = random->getInt(dungeon_config.room_min_size, dungeon_config.room_max_size);

    int x = random->getInt(0, dungeon_config.map_width - room_width - 1);
    int y = random->getInt(0, dungeon_config.map_height - room_height - 1);

    RectangularRoom new_room = RectangularRoom({x, y}, room_width, room_height);

    if (std::any_of(rooms.begin(), rooms.end(), [&](RectangularRoom room) { return room.intersects(new_room); })) {
      continue;
    }

    map->set_tiles_range(new_room.innerBounds(), {false, TileType::floor});

    place_entities(new_room, dungeon_config.max_monsters_per_room, map);

    if (rooms.empty()) {
      player.set_position(new_room.get_center());
    } else {
      Vector2D previous_room_center = rooms.back().get_center();
      std::vector<Vector2D> tunnel = l_tunnel_between(previous_room_center, new_room.get_center());

      for (const Vector2D position : tunnel) {
        map->set_tiles_at(position, {false, TileType::floor});
      }
    }
    rooms.push_back(new_room);
  }

  return map;
}

constexpr float half_chance = 0.5F;

void place_entities(RectangularRoom room, int max_monsters_per_room, Map* map) {
  auto* random = TCODRandom::getInstance();
  int number_of_monsters = random->getInt(0, max_monsters_per_room);

  for (int i = 0; i < number_of_monsters; i++) {
    int x = random->getInt(room.get_bottom_left().x + 1, room.get_top_right().x - 1);
    int y = random->getInt(room.get_bottom_left().y + 1, room.get_top_right().y - 1);

    // Check theres no other monster on this position in the room
    auto iterator = std::find_if(map->get_entities().begin(), map->get_entities().end(), [&](GameEntity* entity) {
      return entity->get_position() == Vector2D{x, y};
    });

    if (iterator != map->get_entities().end()) {
      continue;
    }

    if (random->getFloat(0.0f, 1.0f) < 0.8f) {
      auto* monster = new GameEntity({x, y}, "o", tcod::ColorRGB{0, 255, 0});
      map->push_entity(monster);
    } else {
      auto* troll = new GameEntity({x, y}, "T", tcod::ColorRGB{0, 255, 0});
      map->push_entity(troll);
    }
  }
}

std::vector<Vector2D> Dungeon::l_tunnel_between(Vector2D start, Vector2D end) {
  auto* random = TCODRandom::getInstance();
  Vector2D corner{0, 0};

  if (random->get(0.0f, 1.0f) < half_chance) {
    corner = {end.x, start.y};
  } else {
    corner = {start.x, end.y};
  }

  std::vector<Vector2D> tunnel{};

  for (const auto&& [x, y] : tcod::BresenhamLine({start.x, start.y}, {corner.x, corner.y})) {
    tunnel.push_back({x, y});
  }

  for (const auto&& [x, y] : tcod::BresenhamLine({corner.x, corner.y}, {end.x, end.y})) {
    tunnel.push_back({x, y});
  }

  return tunnel;
}

}  // namespace cpprl
