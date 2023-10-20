#pragma once
#include <libtcod.hpp>

#include "../game_entity.hpp"
#include "../rectangular_room.hpp"
#include "math.hpp"
#include "nparray.hpp"

namespace cpprl {
enum class TileType {
  wall = 0,
  floor,
};

/**
 * Tile struct. Gives us the type of the tile
 * it's current visibility and if it's been explored.
 */
struct Tile {
  bool explored;
  TileType type;
};

struct TileGraphic {
  TCOD_ConsoleTile light, dark, target;
};

class Map {
 public:
  Map(int width, int height);
  ~Map();
  int get_height() const { return height_; }
  int get_width() const { return width_; }
  bool is_in_bounds(Vector2D position) const;
  bool is_not_in_bounds(Vector2D position) const { return !is_in_bounds(position); }
  bool is_explored(Vector2D position);
  void compute_fov(Vector2D position, int radius);
  bool is_in_fov(Vector2D position);
  /** Sets the tile at position to explored. */
  void set_is_explored(Vector2D position) { tiles_.at(position).explored = true; }
  bool is_walkable(Vector2D position) const;
  Array2D<Tile>& get_tiles() { return tiles_; }
  void set_tiles_range(std::tuple<Vector2D, Vector2D> bounds, Tile tile);
  void set_rooms(std::vector<RectangularRoom> rooms) { _rooms = rooms; }
  RectangularRoom get_first_room() { return _rooms.front(); }
  std::vector<RectangularRoom> get_rooms() { return _rooms; }
  void set_tiles_at(Vector2D position, Tile tile);
  /** Returns the wall tile for this map */
  TileGraphic& get_wall_tile() { return wall_tile_; }
  /** Returns the floor tile for this map */
  TileGraphic& get_floor_tile() { return floor_tile_; }
  /** Render the map */
  void render(tcod::Console& console);
  void set_highlight_tile(Vector2D position) { target_tile_ = position; }
  void toggle_target_mode(float max_range) {
    max_range_ = max_range;
    target_mode_ = !target_mode_;
  }
  bool set_target_tile(Vector2D position, Entity& player);
  Vector2D get_target_tile() { return target_tile_; }

 private:
  /** The wall tile */
  TileGraphic wall_tile_;
  /** The floor tile */
  TileGraphic floor_tile_;
  /** The width and height of this map. */
  int width_, height_;
  /** This maps tiles */
  Array2D<Tile> tiles_;
  /** The tcod map */
  TCODMap tcod_map_;
  std::vector<RectangularRoom> _rooms;
  Vector2D target_tile_ = {0, 0};
  bool target_mode_ = false;
  float max_range_ = 0.0f;
};

}  // namespace cpprl
