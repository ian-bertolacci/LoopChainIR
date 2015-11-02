#include "gtest/gtest.h"
#include "LoopChain.h"
#include <iostream>
#include <utility>

using namespace std;

TEST(LoopChainTest, FourChain) {
  LoopChain chain;
  vector<string> upper, lower;

  lower.push_back( "0" );
  upper.push_back( "N" );
  Box box_1( lower, upper );

  lower.clear();
  upper.clear();

  lower.push_back( "1" );
  upper.push_back( "M" );
  Box box_2( lower, upper );

  lower.clear();
  upper.clear();

  lower.push_back( "2" );
  upper.push_back( "J" );
  Box box_3( lower, upper );

  lower.clear();
  upper.clear();

  lower.push_back( "3" );
  upper.push_back( "K" );
  Box box_4( lower, upper );

  lower.clear();
  upper.clear();

  chain.append( LoopNest( box_1 ) );
  chain.append( LoopNest( box_2 ) );
  chain.append( LoopNest( box_3 ) );
  chain.append( LoopNest( box_4 ) );

  EXPECT_EQ( chain.length(), 4 );

  Box got_box = chain.getNest(0).getBounds();

  EXPECT_EQ( got_box.dimensions(), 1 );
  EXPECT_EQ( got_box.getDimensionLowerBound(0), string("0"));
  EXPECT_EQ( got_box.getDimensionUpperBound(0), string("N"));
  EXPECT_EQ( got_box.getDimensionsBounds(0).first,  string("0"));
  EXPECT_EQ( got_box.getDimensionsBounds(0).second, string("N"));

  got_box = chain.getNest(1).getBounds();

  EXPECT_EQ( got_box.dimensions(), 1 );
  EXPECT_EQ( got_box.getDimensionLowerBound(0), string("1"));
  EXPECT_EQ( got_box.getDimensionUpperBound(0), string("M"));
  EXPECT_EQ( got_box.getDimensionsBounds(0).first,  string("1"));
  EXPECT_EQ( got_box.getDimensionsBounds(0).second, string("M"));

  got_box = chain.getNest(2).getBounds();

  EXPECT_EQ( got_box.dimensions(), 1 );
  EXPECT_EQ( got_box.getDimensionLowerBound(0), string("2"));
  EXPECT_EQ( got_box.getDimensionUpperBound(0), string("J"));
  EXPECT_EQ( got_box.getDimensionsBounds(0).first,  string("2"));
  EXPECT_EQ( got_box.getDimensionsBounds(0).second, string("J"));

  got_box = chain.getNest(3).getBounds();

  EXPECT_EQ( got_box.dimensions(), 1 );
  EXPECT_EQ( got_box.getDimensionLowerBound(0), string("3"));
  EXPECT_EQ( got_box.getDimensionUpperBound(0), string("K"));
  EXPECT_EQ( got_box.getDimensionsBounds(0).first,  string("3"));
  EXPECT_EQ( got_box.getDimensionsBounds(0).second, string("K"));
}
