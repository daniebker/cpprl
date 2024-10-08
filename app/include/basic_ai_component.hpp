#ifndef INCLUDE_BASIC_AI_COMPONENT_HPP_
#define INCLUDE_BASIC_AI_COMPONENT_HPP_

#include <cereal/cereal.hpp>  // for defer
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>
#include <libtcod.hpp>

#include "types/engine_fwd.hpp"
#include "types/entity_fwd.hpp"
#include "types/world_fwd.hpp"

namespace cpprl {

class AIComponent {
 public:
  AIComponent() = default;
  virtual ~AIComponent() = default;

  virtual void update(World& world, Entity* entity) = 0;

  template <class Archive>
  void serialize(Archive&) {}
};

class HostileAI final : public AIComponent {
 public:
  HostileAI() = default;
  virtual ~HostileAI() = default;

  void update(World& world, Entity* entity) override;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<AIComponent>(this));
  }
};

class ConfusionAI final : public AIComponent {
 private:
  int num_turns_;
  std::unique_ptr<AIComponent> old_ai_;

 public:
  ConfusionAI() = default;
  ConfusionAI(int num_turns, std::unique_ptr<AIComponent> old_ai);
  virtual ~ConfusionAI() = default;

  void update(World& world, Entity* entity) override;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<AIComponent>(this), num_turns_, old_ai_);
  }
};
}  // namespace cpprl
// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
// #include <cereal/archives/binary.hpp>
// #include <cereal/archives/json.hpp>
// #include <cereal/archives/xml.hpp>
CEREAL_REGISTER_TYPE(cpprl::HostileAI);
CEREAL_REGISTER_TYPE(cpprl::ConfusionAI);
#endif
