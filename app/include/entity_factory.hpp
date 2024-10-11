#pragma once

#include <libtcod.hpp>
#include <core/math.hpp>

namespace cpprl {
  class Entity;
  class AbstractEntityFactory {
    public:
      virtual ~AbstractEntityFactory() = default;

      virtual Entity* create(SupaRL::Vector2D at_position) = 0;

    protected:
      Entity* create_base(
          const std::string& name, tcod::ColorRGB color, std::string symbol);
  };

  class OrcFactory : public AbstractEntityFactory {
    public:
      Entity* create(SupaRL::Vector2D at_position) override;
  };

  class TrollFactory : public AbstractEntityFactory {
    public:
      Entity* create(SupaRL::Vector2D at_position) override;
  };

  class PlayerFactory : public AbstractEntityFactory {
    public:
      Entity* create(SupaRL::Vector2D at_position) override;
  };
}

