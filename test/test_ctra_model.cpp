/*
 * Copyright 2022 Leidos
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Developed by the Human and Vehicle Ensembles (HIVE) Lab at Virginia Commonwealth University (VCU)
 */

#include <gtest/gtest.h>
#include <cooperative_perception/ctra_model.hpp>
#include <cooperative_perception/units.hpp>
#include <cooperative_perception/angle.hpp>
#include <units.h>

namespace cp = cooperative_perception;

/**
 * Test CTRA nextState function against pure rotation
 */
TEST(TestCtraModel, NextStatePureRotation)
{
  using namespace units::literals;

  const cp::CtraState state{ 0_m, 0_m, 0_mps, cp::Angle(0_rad), 1_rad_per_s, 0_mps_sq };
  const auto next_state{ cp::nextState(state, 0.5_s) };
  const cp::CtraState expected_state{ 0_m, 0_m, 0_mps, cp::Angle(0.5_rad), 1_rad_per_s, 0_mps_sq };

  EXPECT_TRUE(cp::utils::almostEqual(next_state, expected_state));
}

/**
 * Test CTRA nextState function against pure translation
 */
TEST(TestCtraModel, NextStatePureTranslation)
{
  using namespace units::literals;

  const cp::CtraState state{ 0_m, 0_m, 1_mps, cp::Angle(0_rad), 0_rad_per_s, 1_mps_sq };
  const auto next_state{ cp::nextState(state, 0.5_s) };
  const cp::CtraState expected_state{ 0.625_m, 0_m, 1.5_mps, cp::Angle(0_rad), 0_rad_per_s, 1_mps_sq };

  EXPECT_TRUE(cp::utils::almostEqual(next_state, expected_state));
}

/**
 * Test CTRA nextState function against mixed rotation and translation
 */
TEST(TestCtraModel, NextStateRotationAndTranslation)
{
  using namespace units::literals;

  const cp::CtraState state{ 0_m, 0_m, 1_mps, cp::Angle(0_rad), 1_rad_per_s, 1_mps_sq };
  const auto next_state{ cp::nextState(state, 0.5_s) };
  const cp::CtraState expected_state{ 0.5967208697966773_m, 0.16305169576864387_m, 1.5_mps,
                                      cp::Angle(0.5_rad),   1_rad_per_s,           1_mps_sq };

  EXPECT_TRUE(cp::utils::almostEqual(cp::utils::roundToDecimalPlace(next_state, 9),
                                     cp::utils::roundToDecimalPlace(expected_state, 9)));
}

TEST(TestCtraModel, Equality)
{
  using namespace units::literals;

  const cp::CtraState state1{ 1.23_m, 2.435_m, 5544_mps, cp::Angle(34656.6543_rad), 595633.555_rad_per_s, 100_mps_sq };
  const cp::CtraState state2{ 1.23_m, 2.435_m, 5544_mps, cp::Angle(34656.6543_rad), 595633.555_rad_per_s, 100_mps_sq };
  const cp::CtraState state3{ 1_m, 2_m, 4_mps, cp::Angle(3_rad), 1.000000000000000000000000001_rad_per_s, 1_mps_sq };
  const cp::CtraState state4{ 1_m, 2_m, 4_mps, cp::Angle(3_rad), 1_rad_per_s, 1_mps_sq };

  EXPECT_TRUE(cp::utils::almostEqual(state1, state1));
  EXPECT_TRUE(cp::utils::almostEqual(state1, state2));
  EXPECT_FALSE(cp::utils::almostEqual(state1, state3));
  EXPECT_TRUE(cp::utils::almostEqual(state3, state4));
}
