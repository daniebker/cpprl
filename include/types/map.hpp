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

  template <class Archive>
  void serialize(Archive& archive) {
    archive(explored, type);
  }
};

struct TileGraphic {
  TCOD_ConsoleTile light, dark, target;
};

template <typename Func>
inline void with_indexes(int width, int height, Func func) {
  for (int y{0}; y < height; ++y) {
    for (int x{0}; x < width; ++x) {
      func(x, y);
    }
  }
}

template <typename Array, typename F>
inline void with_indexes(const Array& array, F func) {
  with_indexes(array.get_width(), array.get_height(), func);
}

class Map {
 public:
  Map() : Map(0, 0){};
  Map(int width, int height);
  ~Map() = default;
  int get_height() const { return height_; }
  int get_width() const { return width_; }
  bool is_in_bounds(Vector2D position) const;
  bool is_not_in_bounds(Vector2D position) const {
    return !is_in_bounds(position);
  }
  bool is_explored(Vector2D position);
  void compute_fov(Vector2D position, int radius);
  bool is_in_fov(Vector2D position);
  /** Sets the tile at position to explored. */
  void set_is_explored(Vector2D position) {
    tiles_.at(position).explored = true;
  }
  bool is_walkable(Vector2D position) const;
  Array2D<Tile>& get_tiles() { return tiles_; }
  void set_tiles_range(std::tuple<Vector2D, Vector2D> bounds, Tile tile);
  void set_rooms(std::vector<RectangularRoom> rooms) { rooms_ = rooms; }
  RectangularRoom get_first_room() { return rooms_.front(); }
  std::vector<RectangularRoom> get_rooms() { return rooms_; }
  void set_tiles_at(Vector2D position, Tile tile);
  /** Returns the wall tile for this map */
  TileGraphic& get_wall_tile() { return wall_tile_; }
  /** Returns the floor tile for this map */
  TileGraphic& get_floor_tile() { return floor_tile_; }
  /** Render the map */
  void render(tcod::Console& console);
  void set_highlight_tile(Vector2D position);
  void toggle_target_mode(float max_range) {
    max_range_ = max_range;
    target_mode_ = !target_mode_;
  }
  bool set_target_tile(Vector2D position, Entity& player);
  Vector2D get_highlight_tile() { return target_tile_; }

  template <class Archive>
  void save(Archive& archive) const {
    archive(width_, height_);
    for (int y{0}; y < get_height(); ++y) {
      for (int x{0}; x < get_width(); ++x) {
        archive(tiles_.at({x, y}).explored);
      }
    }
  }

  template <class Archive>
  void load(Archive& archive) {
    archive(width_, height_);
    // TODO: Failing here because map is 0,0
    for (int y{0}; y < get_height(); ++y) {
      for (int x{0}; x < get_width(); ++x) {
        archive(tiles_.at({x, y}).explored);
      }
    }
  }

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
  std::vector<RectangularRoom> rooms_;
  Vector2D target_tile_ = {0, 0};
  bool target_mode_ = false;
  float max_range_ = 0.0f;
};

}  // namespace cpprl
