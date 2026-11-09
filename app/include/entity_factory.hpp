#ifndef ENTITY_FACTORY_HPP
#define ENTITY_FACTORY_HPP

#include <libtcod.hpp>
#include <memory>

namespace cpprl {
class Entity;
class AbstractEntityFactory {
 public:
  virtual ~AbstractEntityFactory() = default;

  virtual std::unique_ptr<Entity> create() = 0;

 protected:
  std::unique_ptr<Entity> create_base(
      const std::string& name, tcod::ColorRGB color, std::string_view symbol);
};

class OrcFactory : public AbstractEntityFactory {
 public:
  std::unique_ptr<Entity> create() override;
};

class TrollFactory : public AbstractEntityFactory {
 public:
  std::unique_ptr<Entity> create() override;
};

class PlayerFactory : public AbstractEntityFactory {
 public:
  std::unique_ptr<Entity> create() override;
};
}  // namespace cpprl

#endif
