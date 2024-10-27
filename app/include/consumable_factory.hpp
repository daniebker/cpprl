#pragma once

#include <libtcod.hpp>
#include <core/math.hpp>

namespace cpprl {
  class Entity;

  class AbstractConsumableFactory {
    public:
      virtual ~AbstractConsumableFactory() = default;

      virtual Entity* create(SupaRL::Vector2D at_position) = 0;

    protected:
      Entity* create_base(
          std::string name, tcod::ColorRGB color, std::string symbol,SupaRL::Vector2D at_position);
  };

  class HealthPotionFactory : public AbstractConsumableFactory {
    public:
      Entity* create(SupaRL::Vector2D at_position) override;
  };

  class FireballScrollFactory : public AbstractConsumableFactory {
    public:
      Entity* create(SupaRL::Vector2D at_position) override;
  };

  class LightningScrollFactory : public AbstractConsumableFactory {
    public:
      Entity* create(SupaRL::Vector2D at_position) override;
  };

  class ConfusionScrollFactory : public AbstractConsumableFactory {
    public:
      Entity* create(SupaRL::Vector2D at_position) override;
  };
}

