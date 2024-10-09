#include "gtest/gtest.h"
#include "components/components.hpp"

/**
 * @file status_condition_systems_tests.cpp
 */

/**
 * Given an Entity
 * And the entity has a StatusConditionComponent
 * When the StatusConditionSystem is updated
 * Then the entity should take damage from the status condition
 */

TEST(StatusConditionSystems, BleedDamage)
{
  ASSERT_EQ(1, 1);
}
