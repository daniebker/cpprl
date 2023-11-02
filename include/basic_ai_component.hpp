#ifndef INCLUDE_BASIC_AI_COMPONENT_HPP_
#define INCLUDE_BASIC_AI_COMPONENT_HPP_

#include <libtcod.hpp>

#include "persistent.hpp"
#include "types/engine_fwd.hpp"
#include "types/entity_fwd.hpp"
#include "types/world_fwd.hpp"

namespace cpprl {

class AIComponent : public Persistent {
 public:
  AIComponent(){};
  virtual ~AIComponent() = default;
  virtual void update(World& world, Entity* entity) = 0;
  static AIComponent* create(TCODZip& zip);
  virtual void load(TCODZip& zip) = 0;
  virtual void save(TCODZip& zip) = 0;

 protected:
  enum AiType { HOSTILE, CONFUSED };
};

class HostileAI final : public AIComponent {
 public:
  HostileAI() : AIComponent(){};
  void update(World& world, Entity* entity) override;
  void load(TCODZip& zip) override;
  void save(TCODZip& zip) override;
};

class ConfusionAI final : public AIComponent {
 private:
  int num_turns_;
  AIComponent* old_ai_;

 public:
  ConfusionAI(int num_turns, AIComponent* old_ai)
      : AIComponent(), num_turns_(num_turns), old_ai_(old_ai) {}
  ~ConfusionAI() override = default;
  void update(World& world, Entity* entity) override;
  void load(TCODZip& zip) override;
  void save(TCODZip& zip) override;
};
}  // namespace cpprl

#endif
