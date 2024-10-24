#pragma once
#include <vector>

#include <core/math.hpp>

namespace cpprl {

  template <typename T>
    class Array2D {
      public:
        Array2D(int rows, int cols, T initialValue) : rows_(rows), cols_(cols) {
          data_.resize(rows_, std::vector<T>(cols_, initialValue));
        }

        [[deprecated("Use at(SupaRL::Vector2D position) instead")]] T& at(int row, int col) { return at({row, col}); }

        const T& at(SupaRL::Vector2D position) const {
          int row = position.x;
          int col = position.y;
          if (row < 0 || row >= rows_ || col < 0 || col >= cols_) {
            throw std::out_of_range("Array2D index out of bounds");
          }
          return data_[row][col];
        }

        T& at(SupaRL::Vector2D position) {
          return const_cast<T&>(static_cast<const Array2D*>(this)->at(position));
        }

        void set(SupaRL::Vector2D position, T value) { data_[position.x][position.y] = value; }

        void set_range(std::tuple<SupaRL::Vector2D, SupaRL::Vector2D> bounds, T value) {
          SupaRL::Vector2D bottom_left, top_right;
          std::tie(bottom_left, top_right) = bounds;
          for (int x = bottom_left.x; x < top_right.x; x++) {
            for (int y = bottom_left.y; y < top_right.y; y++) {
              set({x, y}, value);
            }
          }
        }

        int getRows() const { return rows_; }

        int getCols() const { return cols_; }

      private:
        int rows_;
        int cols_;
        std::vector<std::vector<T>> data_;
    };
}

