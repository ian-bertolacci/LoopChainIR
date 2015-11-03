/*******************************************************************************
\file LoopNest_test.cpp
\autors Ian J. Bertolacci

\purpose
To perform unit testsing on the LoopNest data structure.

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/

#include "gtest/gtest.h"
#include "LoopNest.h"
#include <iostream>
#include <utility>

using namespace std;

TEST(LoopNestTest, ConstructFromBox) {
  string lower[1] = { "0" };
  string upper[1] = { "1" };

  Box box( lower, upper, 1 );
  LoopNest nest( box );
  Box got_box = nest.getDomain();

  EXPECT_EQ( got_box.dimensions(), box.dimensions() );
  EXPECT_EQ( got_box.getLowerBound(0), box.getLowerBound(0) );
  EXPECT_EQ( got_box.getUpperBound(0), box.getUpperBound(0) );
}
