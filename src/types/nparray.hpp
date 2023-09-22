#ifndef CPPRL_TYPES_NPARRAY_HPP_
#define CPPRL_TYPES_NPARRAY_HPP_
#include <vector>
#include "../math.hpp"

namespace cpprl {

template <typename T>
class Array2D {
 public:
  Array2D(int rows, int cols) : rows_(rows), cols_(cols) { data_.resize(rows_, std::vector<T>(cols_)); }

  [[deprecated("Use at(Vector2D position) instead")]]
  T& at(int row, int col) {
    this->at({row, col});
  }

  T& at(Vector2D position) {
    int row = position.x;
    int col = position.y;
    if (row < 0 || row >= rows_ || col < 0 || col >= cols_) {
      throw std::out_of_range("Array2D index out of bounds");
    }
    return data_[row][col];
  }

  int getRows() const { return rows_; }

  int getCols() const { return cols_; }

 private:
  int rows_;
  int cols_;
  std::vector<std::vector<T>> data_;
};

}  // namespace cpprl

#endif  // CPPRL_TYPES_NPARRAY_HPP_
