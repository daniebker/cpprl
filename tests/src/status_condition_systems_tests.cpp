#include "gtest/gtest.h"
#include "../../lib/include/components/components.hpp"

/**
 * @file status_condition_systems_tests.cpp
 */

/**
 * Given an Entity
 * And the entity has a StatusConditionComponent
 * When the StatusConditionSystem is updated
 * Then the entity should take damage from the status condition
 */

TEST(StatusConditionSystem, TakeDamage) {
  // Setup
  auto entity = 1;
  SupaRL::StatusConditionComponent statusConditionComponent = {
    .damege_per_turn_= 1,
    .turns_remaining_= 1,
    .turns_until_update_= 1,
    .name_= "Bleed"
  };
  // Add the StatusConditionComponent to the entity
  // AddEntityComponent(entity, statusConditionComponent);

  // Update the StatusConditionSystem
  // StatusConditionSystem::Update();

  // Verify the entity has taken damage
  // EXPECT_EQ(GetEntityHealth(entity), 99);
}
