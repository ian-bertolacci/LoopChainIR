/*! ****************************************************************************
\file RectangularDomain_test.cpp
\autors Ian J. Bertolacci

\brief
To perform unit testsing on the RectangularDomain data structure.

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/

#include "gtest/gtest.h"
#include "RectangularDomain.hpp"
#include <iostream>
#include <utility>

using namespace std;

TEST(RectangularDomainTest, Test_Getters_1D ) {
  string lower[1] = { "0" };
  string upper[1] = { "1" };

  RectangularDomain domain( lower, upper, 1);

  EXPECT_EQ( domain.dimensions(), 1 );
  EXPECT_EQ( domain.symbolics(), 0 );
  EXPECT_EQ( domain.getLowerBound(0), "0" );
  EXPECT_EQ( domain.getUpperBound(0), "1" );
}

TEST(RectangularDomainTest, Test_Getters_2D) {
  string lower[2] = { "0", "0" };
  string upper[2] = { "1", "1" };

  RectangularDomain domain( lower, upper, 2 );

  EXPECT_EQ( domain.dimensions(), 2 );
  EXPECT_EQ( domain.symbolics(), 0 );
  EXPECT_EQ( domain.getLowerBound(0), "0" );
  EXPECT_EQ( domain.getUpperBound(0), "1" );
  EXPECT_EQ( domain.getLowerBound(1), "0" );
  EXPECT_EQ( domain.getUpperBound(1), "1" );
}

TEST(RectangularDomainTest, Test_Getters_2D_Symbols) {
  string lower[2] = { "0", "0" };
  string upper[2] = { "1", "N" };
  string symbols[1] = { "N" };

  RectangularDomain domain( lower, upper, 2, symbols, 1 );

  EXPECT_EQ( domain.dimensions(), 2 );
  EXPECT_EQ( domain.symbolics(), 1 );
  EXPECT_EQ( domain.getLowerBound(0), "0" );
  EXPECT_EQ( domain.getUpperBound(0), "1" );
  EXPECT_EQ( domain.getLowerBound(1), "0" );
  EXPECT_EQ( domain.getUpperBound(1), "N" );
  EXPECT_EQ( domain.getSymbol(0), "N" );
}

TEST(RectangularDomainTest, Test_Getters_2D_Symbols_2) {
  string lower[2] = { "L", "0" };
  string upper[2] = { "1", "N" };
  string symbols[2] = { "L", "N" };

  RectangularDomain domain( lower, upper, 2, symbols, 2 );

  EXPECT_EQ( domain.dimensions(), 2 );
  EXPECT_EQ( domain.symbolics(), 2 );
  EXPECT_EQ( domain.getLowerBound(0), "L" );
  EXPECT_EQ( domain.getUpperBound(0), "1" );
  EXPECT_EQ( domain.getLowerBound(1), "0" );
  EXPECT_EQ( domain.getUpperBound(1), "N" );
  EXPECT_EQ( domain.getSymbol(0), "L" );
  EXPECT_EQ( domain.getSymbol(1), "N" );
}
