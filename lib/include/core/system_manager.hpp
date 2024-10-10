#pragma once

#include "system.hpp"
#include "types.hpp"
#include <cassert>
#include <memory>
#include <unordered_map>

namespace SupaRL
{
  class SystemManager
  {
    public:
      template<typename T>
        std::shared_ptr<T> register_system()
        {
          const char* typeName = typeid(T).name();

          assert(systems_.find(typeName) == systems_.end() && "Registering system more than once.");

          auto system = std::make_shared<T>();
          systems_.insert({typeName, system});
          return system;
        }

      template<typename T>
        void set_signature(Signature signature)
        {
          const char* typeName = typeid(T).name();

          assert(systems_.find(typeName) != systems_.end() && "System used before registered.");

          signatures_.insert({typeName, signature});
        }

      void entity_destroyed(Entity entity)
      {
        for (auto const& pair : systems_)
        {
          auto const& system = pair.second;


          system->entities_.erase(entity);
        }
      }

      void entity_signature_changed(Entity entity, Signature entitySignature)
      {
        std::cout << "Entity signature changed " << entity << std::endl;
        for (auto const& pair : systems_)
        {
          auto const& type = pair.first;
          auto const& system = pair.second;
          std::cout << "System: " << type << std::endl;
          std::cout << "System: " << system << std::endl;
          auto const& systemSignature = signatures_[type];
          std::cout << "Entities: " << system->entities_.size() << std::endl;

          if ((entitySignature & systemSignature) == systemSignature)
          {
            system->entities_.insert(entity);
          }
          else
          {
            std::cout << "Erasing entity" << std::endl;
            system->entities_.erase(entity);
            std::cout << "Entities: " << system->entities_.size() << std::endl;
          }
        }
      }

    private:
      std::unordered_map<const char*, Signature> signatures_{};
      std::unordered_map<const char*, std::shared_ptr<System>> systems_{};
  };
}
