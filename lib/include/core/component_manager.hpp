#pragma once

#include "component_array.hpp"
#include "types.hpp"
#include <iostream>
#include <memory>
#include <unordered_map>


namespace SupaRL
{
  class ComponentManager
  {
    public:
      template<typename T>
        void register_component()
        {
          const char* typeName = typeid(T).name();

          assert(component_types_.find(typeName) == component_types_.end() && "Registering component type more than once.");

          component_types_.insert({typeName, next_component_type});
          component_arrays_.insert({typeName, std::make_shared<ComponentArray<T>>()});

          ++next_component_type;
        }

      template<typename T>
        ComponentType get_component_type()
        {
          const char* typeName = typeid(T).name();

          assert(component_types_.find(typeName) != component_types_.end() && "Component not registered before use.");

          return component_types_[typeName];
        }

      template<typename T>
        void add_component(Entity entity, T component)
        {
          get_component_array<T>()->insert_data(entity, component);
        }

      template<typename T>
        void remove_component(Entity entity)
        {
          get_component_array<T>()->remove_data(entity);
        }

      template<typename T>
        T& get_component(Entity entity)
        {
          return get_component_array<T>()->get_data(entity);
        }

      void entity_destroyed(Entity entity)
      {
        for (auto const& pair : component_arrays_)
        {
          auto const& component = pair.second;

          component->entity_destroyed(entity);
        }
      }

    private:
      std::unordered_map<const char*, ComponentType> component_types_{};
      std::unordered_map<const char*, std::shared_ptr<IComponentArray>> component_arrays_{};
      ComponentType next_component_type{};


      template<typename T>
        std::shared_ptr<ComponentArray<T>> get_component_array()
        {
          const char* typeName = typeid(T).name();

          assert(component_types_.find(typeName) != component_types_.end() && "Component not registered before use.");

          return std::static_pointer_cast<ComponentArray<T>>(component_arrays_[typeName]);
        }
  };
}
