#include "gtest/gtest.h"
#include "LoopNest.h"
#include <iostream>
#include <utility>

using namespace std;

TEST(LoopNestTest, ConstructFromBox) {
  vector<string> lower, upper;
  lower.push_back(string("0"));
  upper.push_back(string("1"));

  Box box( lower, upper );
  LoopNest nest( box );
  Box got_box = nest.getBounds();
  EXPECT_EQ( got_box.dimensions(), box.dimensions() );
  EXPECT_EQ( got_box.getDimensionLowerBound(0), box.getDimensionLowerBound(0) );
  EXPECT_EQ( got_box.getDimensionUpperBound(0), box.getDimensionUpperBound(0) );
  EXPECT_EQ( got_box.getDimensionsBounds(0).first, box.getDimensionsBounds(0).first );
  EXPECT_EQ( got_box.getDimensionsBounds(0).second, box.getDimensionsBounds(0).second );
}
