#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <iostream>
#include <stdexcept>

namespace cpprl {
class Impossible : public std::exception {
 private:
  std::string message_;

 public:
  Impossible(const char* message) : message_(message) {}
  const char* what() const noexcept override { return message_.c_str(); }
};
}  // namespace cpprl
#endif
