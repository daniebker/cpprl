#ifndef CONSUMABLE_FACTORY_HPP
#define CONSUMABLE_FACTORY_HPP

#include <libtcod.hpp>
#include <memory>

namespace cpprl {
class Entity;

class AbstractConsumableFactory {
 public:
  virtual ~AbstractConsumableFactory() = default;

  virtual Entity* create() = 0;

 protected:
  Entity* create_base(
      std::string name, tcod::ColorRGB color, std::string_view symbol);
};

class HealthPotionFactory : public AbstractConsumableFactory {
 public:
  Entity* create() override;
};

class FireballScrollFactory : public AbstractConsumableFactory {
 public:
  Entity* create() override;
};

class LightningScrollFactory : public AbstractConsumableFactory {
 public:
  Entity* create() override;
};

class ConfusionScrollFactory : public AbstractConsumableFactory {
 public:
  Entity* create() override;
};
}  // namespace cpprl

#endif
