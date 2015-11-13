/*******************************************************************************
\file RectangularDomain_test.cpp
\autors Ian J. Bertolacci

\purpose
To perform unit testsing on the RectangularDomain data structure.

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/

#include "gtest/gtest.h"
#include "RectangularDomain.h"
#include <iostream>
#include <utility>

using namespace std;

TEST(RectangularDomainTest, ConstructArrayString_1) {
  string lower[1] = { "0" };
  string upper[1] = { "1" };

  RectangularDomain domain( lower, upper, 1 );

  EXPECT_EQ( domain.dimensions(), 1 );
  EXPECT_EQ( domain.getLowerBound(0), "0" );
  EXPECT_EQ( domain.getUpperBound(0), "1" );
}

TEST(RectangularDomainTest, ConstructArrayString_2) {
  string lower[2] = { "0", "0" };
  string upper[2] = { "1", "1" };

  RectangularDomain domain( lower, upper, 2 );

  EXPECT_EQ( domain.dimensions(), 2 );
  EXPECT_EQ( domain.getLowerBound(0), "0" );
  EXPECT_EQ( domain.getUpperBound(0), "1" );
  EXPECT_EQ( domain.getLowerBound(1), "0" );
  EXPECT_EQ( domain.getUpperBound(1), "1" );
}
