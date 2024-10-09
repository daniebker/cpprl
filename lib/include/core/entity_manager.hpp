#pragma once

#include "types.hpp"
#include <array>
#include <cassert>
#include <queue>
#include <iostream>


namespace SupaRL {
  class EntityManager
  {
    public:
      EntityManager()
      {
        for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
        {
          available_entities_.push(entity);
        }
      }

      Entity create_entity()
      {
        assert(living_entity_count_ < MAX_ENTITIES && "Too many entities in existence.");

        Entity id = available_entities_.front();
        available_entities_.pop();
        ++living_entity_count_;

        return id;
      }

      void destroy_entity(Entity entity)
      {
        assert(entity < MAX_ENTITIES && "Entity out of range.");

        signatures_[entity].reset();
        available_entities_.push(entity);
        --living_entity_count_;
      }

      void set_signature(Entity entity, Signature signature)
      {
        std::cout << "Setting signature for entity " << entity << std::endl;
        std::cout << "Num Signatures: " << signatures_.size() << std::endl;
        std::cout << "Signature: " << signatures_[entity] << std::endl;
        assert(entity < MAX_ENTITIES && "Entity out of range.");

        signatures_[entity] = signature;
      }

      Signature get_signature(Entity entity)
      {
        assert(entity < MAX_ENTITIES && "Entity out of range.");

        return signatures_[entity];
      }

    private:
      std::queue<Entity> available_entities_{};
      std::array<Signature, MAX_ENTITIES> signatures_{};
      uint32_t living_entity_count_{};
  };

}
