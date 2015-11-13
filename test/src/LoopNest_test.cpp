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

TEST(LoopNestTest, ConstructFromRectangularDomain) {
  string lower[1] = { "0" };
  string upper[1] = { "1" };

  RectangularDomain domain( lower, upper, 1 );
  LoopNest nest( domain );
  RectangularDomain got_domain = nest.getDomain();

  EXPECT_EQ( got_domain.dimensions(), domain.dimensions() );
  EXPECT_EQ( got_domain.getLowerBound(0), domain.getLowerBound(0) );
  EXPECT_EQ( got_domain.getUpperBound(0), domain.getUpperBound(0) );
}
