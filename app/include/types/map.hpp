#pragma once

#include <libtcod.hpp>

#include "../game_entity.hpp"
#include "../rectangular_room.hpp"
#include <core/math.hpp>
#include "nparray.hpp"
#include "types/tile.hpp"

namespace cpprl {

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
      bool is_in_bounds(SupaRL::Vector2D position) const;
      bool is_not_in_bounds(SupaRL::Vector2D position) const {
        return !is_in_bounds(position);
      }
      bool is_explored(SupaRL::Vector2D position);
      void compute_fov(SupaRL::Vector2D position, int radius);
      bool is_in_fov(SupaRL::Vector2D position);
      /** Sets the tile at position to explored. */
      void set_is_explored(SupaRL::Vector2D position) {
        tiles_.at(position).explored = true;
      }
      bool is_walkable(SupaRL::Vector2D position) const;
      Array2D<Tile>& get_tiles() { return tiles_; }
      void set_tiles_range(std::tuple<SupaRL::Vector2D, SupaRL::Vector2D> bounds, Tile tile);
      void set_rooms(std::vector<RectangularRoom> rooms) { _rooms = rooms; }
      void set_down_stairs_location(SupaRL::Vector2D position) {
        down_stairs_location_ = position;
      }
      RectangularRoom get_first_room() { return _rooms.front(); }
      std::vector<RectangularRoom> get_rooms() { return _rooms; }
      void set_tiles_at(SupaRL::Vector2D position, Tile tile);
      /** Returns the wall tile for this map */
      TileGraphic& get_wall_tile() { return wall_tile_; }
      /** Returns the floor tile for this map */
      TileGraphic& get_floor_tile() { return floor_tile_; }

      /**
       * @brief Returns the tile graphic for the given tile type.
       * @param type The tile type.
       * @return The tile graphic.
       */
      TileGraphic& get_tile_graphic(TileType type) {
        if (type == TileType::wall) {
          return wall_tile_;
        } else if (type == TileType::floor) {
          return floor_tile_;
        } else if (type == TileType::down_stairs) {
          return downstairs_tile_;
        } else {
          return wall_tile_;
        }
      }

      /** Render the map */
      void render(tcod::Console& console);
      void set_highlight_tile(SupaRL::Vector2D position);
      void toggle_target_mode(float max_range) {
        max_range_ = max_range;
        target_mode_ = !target_mode_;
      }
      bool set_target_tile(SupaRL::Vector2D position, Entity& player);
      SupaRL::Vector2D get_highlight_tile() { return target_tile_; }

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
      TileGraphic downstairs_tile_;
      /** The width and height of this map. */
      int width_, height_;
      /** This maps tiles */
      Array2D<Tile> tiles_;

      /**
       * @brief tcod map used for fov calculations
       */
      TCODMap tcod_map_;
      std::vector<RectangularRoom> _rooms;
      SupaRL::Vector2D target_tile_ = {0, 0};
      bool target_mode_ = false;
      float max_range_ = 0.0f;
      SupaRL::Vector2D down_stairs_location_{0, 0};
  };

}
