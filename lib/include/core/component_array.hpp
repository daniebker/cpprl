#pragma once

#include "types.hpp"
#include <array>
#include <cassert>
#include <unordered_map>

namespace SupaRL {
  class IComponentArray
  {
    public:
      virtual ~IComponentArray() = default;
      virtual void entity_destroyed(Entity entity) = 0;
  };


  template<typename T>
    class ComponentArray : public IComponentArray
  {
    public:
      void insert_data(Entity entity, T component)
      {
        assert(entity_to_index_map_.find(entity) == entity_to_index_map_.end() && "Component added to same entity more than once.");

        // Put new entry at end
        size_t newIndex = size_;
        entity_to_index_map_[entity] = newIndex;
        index_to_entity_map_[newIndex] = entity;
        component_array_[newIndex] = component;
        ++size_;
      }

      void remove_data(Entity entity)
      {
        assert(entity_to_index_map_.find(entity) != entity_to_index_map_.end() && "Removing non-existent component.");

        // Copy element at end into deleted element's place to maintain density
        size_t indexOfRemovedEntity = entity_to_index_map_[entity];
        size_t indexOfLastElement = size_ - 1;
        component_array_[indexOfRemovedEntity] = component_array_[indexOfLastElement];

        // Update map to point to moved spot
        Entity entityOfLastElement = index_to_entity_map_[indexOfLastElement];
        entity_to_index_map_[entityOfLastElement] = indexOfRemovedEntity;
        index_to_entity_map_[indexOfRemovedEntity] = entityOfLastElement;

        entity_to_index_map_.erase(entity);
        index_to_entity_map_.erase(indexOfLastElement);

        --size_;
      }

      T& get_data(Entity entity)
      {
        assert(entity_to_index_map_.find(entity) != entity_to_index_map_.end() && "Retrieving non-existent component.");

        return component_array_[entity_to_index_map_[entity]];
      }

      void entity_destroyed(Entity entity) override
      {
        if (entity_to_index_map_.find(entity) != entity_to_index_map_.end())
        {
          remove_data(entity);
        }
      }

    private:
      std::array<T, MAX_ENTITIES> component_array_{};
      std::unordered_map<Entity, size_t> entity_to_index_map_{};
      std::unordered_map<size_t, Entity> index_to_entity_map_{};
      size_t size_{};
  };

}
