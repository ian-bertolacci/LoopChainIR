/*! ****************************************************************************
\file LoopNest_test.cpp
\autors Ian J. Bertolacci

\brief
To perform unit testsing on the LoopNest data structure.

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/

#include "gtest/gtest.h"
#include "LoopNest.hpp"
#include <iostream>
#include <utility>

using namespace std;

TEST(LoopNestTest, Test_Getters_1D) {
  string lower[1] = { "0" };
  string upper[1] = { "1" };

  RectangularDomain domain( lower, upper, 1 );
  LoopNest nest( domain );
  RectangularDomain got_domain = nest.getDomain();

  EXPECT_EQ( got_domain.dimensions(), domain.dimensions() );
  EXPECT_EQ( got_domain.symbolics(), domain.symbolics() );

  for( RectangularDomain::size_type d = 0; d < got_domain.dimensions(); d += 1){
    EXPECT_EQ( got_domain.getLowerBound(d), domain.getLowerBound(d) );
    EXPECT_EQ( got_domain.getUpperBound(d), domain.getUpperBound(d) );
  }

  for( RectangularDomain::size_type s = 0; s < got_domain.symbolics(); s += 1 ){
    EXPECT_EQ( got_domain.getSymbol(s), domain.getSymbol(s) );
  }
}

TEST(LoopNestTest, Test_Getters_1D_Symbols) {
  string lower[1] = { "0" };
  string upper[1] = { "N" };
  string symbols[1] = { "N" };

  RectangularDomain domain( lower, upper, 1 );
  LoopNest nest( domain );
  RectangularDomain got_domain = nest.getDomain();

  EXPECT_EQ( got_domain.dimensions(), domain.dimensions() );
  EXPECT_EQ( got_domain.symbolics(), domain.symbolics() );

  for( RectangularDomain::size_type d = 0; d < got_domain.dimensions(); d += 1){
    EXPECT_EQ( got_domain.getLowerBound(d), domain.getLowerBound(d) );
    EXPECT_EQ( got_domain.getUpperBound(d), domain.getUpperBound(d) );
  }

  for( RectangularDomain::size_type s = 0; s < got_domain.symbolics(); s += 1 ){
    EXPECT_EQ( got_domain.getSymbol(s), domain.getSymbol(s) );
  }
}
