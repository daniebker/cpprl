#ifndef _JSON_SERIALIZER_STRATEGY_HPP_INCLUDED
#define _JSON_SERIALIZER_STRATEGY_HPP_INCLUDED

#include <cereal/archives/json.hpp>
#include <fstream>

#include "world.hpp"

namespace cpprl {
namespace serialization {
class JsonSerializerStrategy {
 private:
  std::string filename_;

 public:
  JsonSerializerStrategy(std::string filename) : filename_(filename){};
  ~JsonSerializerStrategy() = default;

  void serialize(World& args) {
    std::ofstream ofs(filename_, std::ios::binary);
    cereal::JSONOutputArchive archive(ofs);

    archive(args);
  };

  void deserialize(World& args) {
    std::ifstream ifs(filename_, std::ios::binary);
    cereal::JSONInputArchive archive(ifs);

    archive(args);
  };
};
}  // namespace serialization
}  // namespace cpprl
#endif
