/*******************************************************************************
\file Box_test.cpp
\autors Ian J. Bertolacci

\purpose
To perform unit testsing on the Box data structure.

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/

#include "gtest/gtest.h"
#include "Box.h"
#include <iostream>
#include <utility>

using namespace std;

TEST(BoxTest, ConstructArrayString_1) {
  string lower[1] = { "0" };
  string upper[1] = { "1" };

  Box box( lower, upper, 1 );

  EXPECT_EQ( box.dimensions(), 1 );
  EXPECT_EQ( box.getLowerBound(0), "0" );
  EXPECT_EQ( box.getUpperBound(0), "1" );
}

TEST(BoxTest, ConstructArrayString_2) {
  string lower[2] = { "0", "0" };
  string upper[2] = { "1", "1" };

  Box box( lower, upper, 2 );

  EXPECT_EQ( box.dimensions(), 2 );
  EXPECT_EQ( box.getLowerBound(0), "0" );
  EXPECT_EQ( box.getUpperBound(0), "1" );
  EXPECT_EQ( box.getLowerBound(1), "0" );
  EXPECT_EQ( box.getUpperBound(1), "1" );
}
