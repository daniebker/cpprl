#ifndef CPPRL_TYPES_NPARRAY_HPP_
#define CPPRL_TYPES_NPARRAY_HPP_
#include <vector>

namespace cpprl {

template <typename T>
class Array2D {
 public:
  Array2D(int rows, int cols) : rows_(rows), cols_(cols) { data_.resize(rows_, std::vector<T>(cols_)); }

  // Accessor functions
  T& at(int row, int col) {
    if (row < 0 || row >= rows_ || col < 0 || col >= cols_) {
      throw std::out_of_range("Array2D index out of bounds");
    }
    return data_[row][col];
  }

  const T& at(int row, int col) const {
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
