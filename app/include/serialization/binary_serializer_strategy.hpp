#ifndef _BINARY_SERIALIZER_STRATEGY_HPP_INCLUDED
#define _BINARY_SERIALIZER_STRATEGY_HPP_INCLUDED

#include <cereal/archives/portable_binary.hpp>
#include <fstream>

#include "world.hpp"

namespace cpprl {
namespace serialization {
class BinarySerializerStrategy {
 private:
  std::string filename_;

 public:
  BinarySerializerStrategy(std::string filename) : filename_(filename){};
  ~BinarySerializerStrategy() = default;

  void serialize(World& args) {
    std::ofstream ofs(filename_, std::ios::binary);
    cereal::BinaryOutputArchive archive(ofs);

    archive(args);
  };

  void deserialize(World& args) {
    std::ifstream ifs(filename_, std::ios::binary);
    cereal::BinaryInputArchive archive(ifs);

    archive(args);
  };
};
}  // namespace serialization
}  // namespace cpprl
#endif
