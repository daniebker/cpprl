#ifndef _TILE_HPP_
#define _TILE_HPP_

namespace cpprl {
/**
 * @brief The type of tile.
 */
enum class TileType { wall = 0, floor, down_stairs };

/**
 * @brief Tile struct. Gives us the type of the tile
 * it's current visibility and if it's been explored.
 */
struct Tile {
  /**
   * @brief If this tile has already been explored.
   */
  bool explored;

  /**
   * @brief The tiles type. Maps directly to a TileGraphic.
   * @see TileGraphic
   */
  TileType type;

  /**
   * @brief if light can pass through this tile.
   */
  bool is_transparent = false;

  /**
   * @brief if this tile blocks movement.
   */
  bool blocking;

  Tile() = default;
  Tile(bool explored, TileType type, bool blocking, bool is_transparent)
      : explored(explored),
        type(type),
        is_transparent(is_transparent),
        blocking(blocking) {}

  /**
   * @brief Cereal serialization function.
   */
  template <class Archive>
  void serialize(Archive& archive) {
    archive(explored, type);
  }
};

/**
 * @brief TileGraphic struct. Gives us the graphic for a tile.
 * Trying to implement an overengineered Flyweight pattern.
 */
struct TileGraphic {
  TCOD_ConsoleTile light, dark, target;
};

/**
 * @brief The floor tile.
 */
inline static Tile FLOOR_TILE = Tile{false, TileType::floor, false, true};

/**
 * @brief The wall tile.
 */
inline static Tile WALL_TILE = Tile{false, TileType::wall, true, false};

/**
 * @brief The down stairs tile.
 */
inline static Tile DOWN_STAIRS_TILE =
    Tile{false, TileType::down_stairs, false, true};
}  // namespace cpprl
#endif
