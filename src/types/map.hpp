#pragma once

#include <libtcod.hpp>

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
  TileType tile;
};

struct TileGraphic {
  TCOD_ConsoleTile light, dark;
};

class Map {
 public:
  Map(int width, int height);
  ~Map();
  void render(tcod::Console& console);
  int get_height() const { return height_; }
  int get_width() const { return width_; }
  bool is_in_bounds(Vector2D position) const;
  bool is_not_in_bounds(Vector2D position) const { return !is_in_bounds(position); }
  bool is_explored(Vector2D position);
  bool is_not_explored(Vector2D position) { return !is_explored(position); }
  void compute_fov(Vector2D position, int radius);
  bool is_in_fov(Vector2D position);
  /** Sets the tile at position to explored. */
  void set_is_explored(Vector2D position) { tiles_.at(position).explored = true; }
  Array2D<Tile>& get_tiles() { return tiles_; }
  void set_tiles_at(std::tuple<Vector2D, Vector2D> bounds, Tile tile);
  /** Returns the wall tile for this map */
  TileGraphic& get_wall_tile() { return wall_tile_; }
  /** Returns the floor tile for this map */
  TileGraphic& get_floor_tile() { return floor_tile_; }

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
};

}  // namespace cpprl
