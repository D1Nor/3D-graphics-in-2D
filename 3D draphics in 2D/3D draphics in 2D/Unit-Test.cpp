#include "googletest/googlemock/include/gmock/gmock.h"
#include "googletest/googletest/include/gtest/gtest.h"
#include "Final Try 3d-2d"
#include <gtest/gtest.h>

using namespace std;

TEST(TestGroupName, PlayerXYZ) {
    float PlayerX = 1.0;
    float PlayerY = 1.0;
    float PlayerA = 0.0;

    ASSERT_TRUE(PlayerX == 1.0);
    ASSERT_TRUE(PlayerY == 1.0);
    ASSERT_FALSE(PlayerA == 500.0);
}