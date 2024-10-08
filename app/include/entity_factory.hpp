#ifndef ENTITY_FACTORY_HPP
#define ENTITY_FACTORY_HPP

#include <libtcod.hpp>
#include <memory>

namespace cpprl {
class Entity;
class AbstractEntityFactory {
 public:
  virtual ~AbstractEntityFactory() = default;

  virtual Entity* create() = 0;

 protected:
  Entity* create_base(
      const std::string& name, tcod::ColorRGB color, std::string_view symbol);
};

class OrcFactory : public AbstractEntityFactory {
 public:
  Entity* create() override;
};

class TrollFactory : public AbstractEntityFactory {
 public:
  Entity* create() override;
};

class PlayerFactory : public AbstractEntityFactory {
 public:
  Entity* create() override;
};
}  // namespace cpprl

#endif
