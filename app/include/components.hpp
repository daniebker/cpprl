#pragma once

#include <cereal/types/polymorphic.hpp>
#include <libtcod.hpp>

#include "game_entity.hpp"
#include "types/action_result.hpp"
#include <core/math.hpp>
#include "types/world_fwd.hpp"

namespace cpprl {

  class  AttackComponent {
    public:
      AttackComponent() = default;
      explicit AttackComponent(int damage) : damage_(damage) {}
      virtual ~AttackComponent() = default;
      int get_damage() const { return damage_; }
      void boost_damage(int amount) { damage_ += amount; }

      template <class Archive>
        void serialize(Archive& archive) {
          archive(damage_);
        }

    private:
      int damage_;
  };

  class DefenseComponent {
    public:
      DefenseComponent() = default;
      DefenseComponent(int defense, int maxHp)
        : defense_(defense), hp_(maxHp), max_hp_(maxHp) {}
      virtual ~DefenseComponent() = default;

      int get_hp() const { return hp_; }
      int get_max_hp() const { return max_hp_; }
      int get_defense() const { return defense_; }
      void boost_defense(int amount) { defense_ += amount; }

      void take_damage(int damage) { hp_ -= damage; }
      int heal(int amount);
      bool is_dead() const { return hp_ <= 0; }
      bool is_not_dead() const { return !is_dead(); }
      void die(Entity& owner) const;

      template <class Archive>
        void serialize(Archive& archive) {
          archive(defense_, hp_, max_hp_);
        }

    private:
      int defense_;
      int hp_;
      int max_hp_;
  };

  class Container {
    private:
      size_t size_;
      std::vector<Entity*> inventory_;

    public:
      Container() = default;
      explicit Container(int size);
      virtual ~Container() = default;
      bool add(Entity* actor);
      void remove(const Entity* actor);
      std::vector<Entity*> get_inventory() const { return inventory_; }
      size_t get_size() const { return size_; }

      template <class Archive>
        void save(Archive& archive) const {
          archive(size_);
          archive(inventory_.size());
          for (auto& item : inventory_) {
            item->pack(archive);
          }
        }
      template <class Archive>
        void load(Archive& archive) {
          archive(size_);
          int nb_items;
          archive(nb_items);
          for (int i = 0; i < nb_items; i++) {
            Entity* entity = new Entity("", false);
            entity->unpack(archive);
            inventory_.emplace_back(entity);
          }
        }
  };

  class ConsumableComponent {
    public:
      ConsumableComponent() = default;
      virtual ~ConsumableComponent() = default;
      // TODO: should also be an action result
      ActionResult pick_up(Entity* owner, Entity* wearer);
      ActionResult drop(Entity* owner, Entity* wearer);
      virtual ActionResult use(Entity* owner, Entity* wearer, World& world);

      template <class Archive>
        void serialize(Archive&) const {
          // nothing to archive
        }

    protected:
      enum class ConsumableType { HEALER, LIGHTNING_BOLT, CONFUSER, FIREBALL };
  };

  class HealingConsumable final : public ConsumableComponent {
    public:
      HealingConsumable() = default;
      explicit HealingConsumable(int amount);
      ~HealingConsumable() override = default;
      ActionResult use(Entity* owner, Entity* wearer, World& world) override;
      template <class Archive>
        void serialize(Archive& archive) {
          archive(cereal::base_class<ConsumableComponent>(this), amount_);
        }

    private:
      int amount_;
  };

  class LightningBolt final : public ConsumableComponent {
    private:
      float range_;
      float damage_;

    public:
      LightningBolt() = default;
      LightningBolt(float range, float damage) : range_(range), damage_(damage) {}
      ~LightningBolt() override = default;
      ActionResult use(Entity* owner, Entity* wearer, World& world) override;

      template <class Archive>
        void serialize(Archive& archive) {
          archive(cereal::base_class<ConsumableComponent>(this), range_, damage_);
        }
  };

  class FireSpell final : public ConsumableComponent {
    private:
      float max_range_;
      float aoe_;
      float damage_;

    public:
      FireSpell() = default;
      FireSpell(float max_range, float aoe, float damage)
        : max_range_(max_range), aoe_(aoe), damage_(damage) {}
      ~FireSpell() override = default;

      ActionResult use(Entity* owner, Entity* Wearer, World& world) override;
      template <class Archive>
        void serialize(Archive& archive) {
          archive(
              cereal::base_class<ConsumableComponent>(this),
              max_range_,
              aoe_,
              damage_);
        }
  };

  class ConfusionSpell final : public ConsumableComponent {
    private:
      int num_turns_;
      int max_range_;

    public:
      ConfusionSpell() = default;
      ConfusionSpell(int num_turns, int max_range)
        : num_turns_(num_turns), max_range_(max_range) {}
      ~ConfusionSpell() override = default;

      ActionResult use(Entity* owner, Entity* wearer, World& world) override;

      template <class Archive>
        void serialize(Archive& archive) {
          archive(
              cereal::base_class<ConsumableComponent>(this), num_turns_, max_range_);
        }
  };

  /**
   * @brief StatsData
   * Simple data structure to hold stats data
   */
  struct StatsData {
    int xp_;
    int level_;
    int level_up_base_;
    int level_up_factor_;
    int stats_points_;

    template <class Archive>
      void serialize(Archive& archive) {
        archive(xp_, level_, level_up_base_, level_up_factor_, stats_points_);
      }
  };

  /**
   * @brief StatsComponent
   * Component used to manipulate stats data.
   */
  class StatsComponent {
    public:
      StatsComponent() = default;
      explicit StatsComponent(StatsData stats_data) : stats_data_(stats_data) {}
      StatsComponent(
          int xp,
          int level,
          int level_up_base,
          int level_up_factor,
          int stats_points)
        : stats_data_{xp, level, level_up_base, level_up_factor, stats_points} {}
      virtual ~StatsComponent() = default;

      int get_xp() const { return stats_data_.xp_; }
      int get_level() const { return stats_data_.level_; }
      int get_level_up_base() const { return stats_data_.level_up_base_; }
      int get_level_up_factor() const { return stats_data_.level_up_factor_; }
      void reduce_stats_points(int amount) { stats_data_.stats_points_ -= amount; }
      int get_stats_points() const { return stats_data_.stats_points_; }
      int get_next_level_xp() const {
        return stats_data_.level_up_base_ +
          stats_data_.level_ * stats_data_.level_up_factor_;
      }

      void add_xp(int xp);
      void level_up();

      template <class Archive>
        void serialize(Archive& archive) {
          archive(stats_data_);
        }

    private:
      StatsData stats_data_;
  };
}

CEREAL_REGISTER_TYPE(cpprl::HealingConsumable);
CEREAL_REGISTER_TYPE(cpprl::LightningBolt);
CEREAL_REGISTER_TYPE(cpprl::FireSpell);
CEREAL_REGISTER_TYPE(cpprl::ConfusionSpell);
CEREAL_REGISTER_TYPE(cpprl::StatsComponent);
CEREAL_REGISTER_TYPE(cpprl::AttackComponent);
