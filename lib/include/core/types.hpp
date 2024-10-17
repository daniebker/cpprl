#pragma once

#include <bitset>
#include <cstdint>

namespace SupaRL {

  //TODO: Create an inline function to handle combat and register it as an event listener
  // see: https://code.austinmorlan.com/austin/2019-ecs/src/branch/master/Source/Main.cpp#L37
  // or method: https://code.austinmorlan.com/austin/2019-ecs/src/branch/master/Source/Systems/RenderSystem.cpp#L16
#define METHOD_LISTENER(EventType, Listener) EventType, std::bind(&Listener, this, std::placeholders::_1)
#define FUNCTION_LISTENER(EventType, Listener) EventType, std::bind(&Listener, std::placeholders::_1)

  // Source: https://gist.github.com/Lee-R/3839813
  constexpr std::uint32_t fnv1a_32(char const* s, std::size_t count)
  {
    return ((count ? fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u; // NOLINT (hicpp-signed-bitwise)
  }

  constexpr std::uint32_t operator "" _hash(char const* s, std::size_t count)
  {
    return fnv1a_32(s, count);
  }

  using Entity = std::uint32_t;
  const Entity MAX_ENTITIES = 5000;
  using ComponentType = std::uint8_t;
  const ComponentType MAX_COMPONENTS = 32;
  using Signature = std::bitset<MAX_COMPONENTS>;

  // Events
  using EventId = std::uint32_t;
  using ParamId = std::uint32_t;

  // TODO: Make these easier to define and use (macro?)
  // TODO: Add some kind of enforcement/automation that a SetParam type and a GetParam type match
  namespace Events::Combat {
    const EventId ATTACK = "Events::Combat::ATTACK"_hash;
    const EventId SPELL = "Events::Combat::SPELL"_hash;
    const ParamId ATTACKER = "Events::Combat::Attack::Attacker"_hash;
    const ParamId DEFENDER = "Events::Combat::Attack::Defender"_hash;
  }

  namespace Events::Combat::Damage{
    const EventId DEALT = "Events::Combat::Damage::DEALT"_hash;
    const ParamId DAMAGE = "Events::Combat::Damage::Damage"_hash;
    const ParamId ATTACKER = "Events::Combat::Damage::Attacker"_hash;
    const ParamId DEFENDER = "Events::Combat::Damage::Defender"_hash;
  }

  namespace Events::Combat::Die{
    const EventId DIE = "Events::Combat::Die::DIE"_hash;
    const ParamId ENTITY = "Events::Combat::Die::Entity"_hash;
  }

  namespace Events::Combat::Spell{
    const ParamId TARGET = "Events::Combat::Spell::Target"_hash;
    const ParamId POWER = "Events::Combat::Spell::Power"_hash;
  }

  namespace Events::StatusCondition {
    const EventId APPLY = "Events::StatusCondition::APPLY"_hash;
    const EventId REMOVE = "Events::StatusCondition::REMOVE"_hash;
  }

  namespace Events::StatusCondition {
    const ParamId NAME = "Events::StatusCondition::Name"_hash;
    const ParamId DAMAGE_PER_TICK = "Events::StatusCondition::DamagePerTick"_hash;
    const ParamId CONDITION = "Events::StatusCondition::Condition"_hash;
  }

}
