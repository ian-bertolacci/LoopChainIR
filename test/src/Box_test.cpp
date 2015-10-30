#include "gtest/gtest.h"
#include "Box.h"
#include <iostream>
#include <utility>

using namespace std;

TEST(BoxTest, ConstructVectorStrings_1) {
  vector<string> lower, upper;
  lower.push_back(string("0 <= x"));
  upper.push_back(string("x <= 1"));

  Box box( lower, upper );
  EXPECT_EQ( box.dimensions(), 1 );
  EXPECT_EQ( box.getDimensionLowerBound(0), string("0 <= x"));
  EXPECT_EQ( box.getDimensionUpperBound(0), string("x <= 1"));
  EXPECT_EQ( box.getDimensionsBounds(0).first,  string("0 <= x"));
  EXPECT_EQ( box.getDimensionsBounds(0).second, string("x <= 1"));
}

TEST(BoxTest, ConstructVectorStrings_2) {
  vector<string> lower, upper;
  lower.push_back(string("0 <= x"));
  lower.push_back(string("0 <= y"));
  upper.push_back(string("x <= 1"));
  upper.push_back(string("y <= 1"));

  Box box( lower, upper );
  EXPECT_EQ( box.dimensions(), 2 );
  EXPECT_EQ( box.getDimensionLowerBound(0), string("0 <= x"));
  EXPECT_EQ( box.getDimensionUpperBound(0), string("x <= 1"));
  EXPECT_EQ( box.getDimensionLowerBound(1), string("0 <= y"));
  EXPECT_EQ( box.getDimensionUpperBound(1), string("y <= 1"));
  EXPECT_EQ( box.getDimensionsBounds(0).first,  string("0 <= x"));
  EXPECT_EQ( box.getDimensionsBounds(0).second, string("x <= 1"));
  EXPECT_EQ( box.getDimensionsBounds(1).first,  string("0 <= y"));
  EXPECT_EQ( box.getDimensionsBounds(1).second, string("y <= 1"));

}

TEST(BoxTest, ConstructVectorPairs_1) {
  vector< pair<string,string> > bounds;
  bounds.push_back( pair<string,string>( string("0 <= x"), string("x <= 1") ) );

  Box box( bounds );
  EXPECT_EQ( box.dimensions(), 1 );
  EXPECT_EQ( box.getDimensionLowerBound(0), string("0 <= x"));
  EXPECT_EQ( box.getDimensionUpperBound(0), string("x <= 1"));
  EXPECT_EQ( box.getDimensionsBounds(0).first,  string("0 <= x"));
  EXPECT_EQ( box.getDimensionsBounds(0).second, string("x <= 1"));
}

TEST(BoxTest, ConstructVectorPairs_2) {
  vector< pair<string,string> > bounds;
  bounds.push_back( pair<string,string>( string("0 <= x"), string("x <= 1") ) );
  bounds.push_back( pair<string,string>( string("0 <= y"), string("y <= 1") ) );

  Box box( bounds );
  EXPECT_EQ( box.dimensions(), 2 );
  EXPECT_EQ( box.getDimensionLowerBound(0), string("0 <= x"));
  EXPECT_EQ( box.getDimensionUpperBound(0), string("x <= 1"));
  EXPECT_EQ( box.getDimensionLowerBound(1), string("0 <= y"));
  EXPECT_EQ( box.getDimensionUpperBound(1), string("y <= 1"));
  EXPECT_EQ( box.getDimensionsBounds(0).first,  string("0 <= x"));
  EXPECT_EQ( box.getDimensionsBounds(0).second, string("x <= 1"));
  EXPECT_EQ( box.getDimensionsBounds(1).first,  string("0 <= y"));
  EXPECT_EQ( box.getDimensionsBounds(1).second, string("y <= 1"));
}

TEST(BoxTest, ConstructArrayString_1) {
  string lower[1] = { string("0 <= x") };
  string upper[1] = { string("x <= 1") };

  Box box( lower, upper, 1 );
  EXPECT_EQ( box.dimensions(), 1 );
  EXPECT_EQ( box.getDimensionLowerBound(0), string("0 <= x"));
  EXPECT_EQ( box.getDimensionUpperBound(0), string("x <= 1"));
  EXPECT_EQ( box.getDimensionsBounds(0).first,  string("0 <= x"));
  EXPECT_EQ( box.getDimensionsBounds(0).second, string("x <= 1"));
}

TEST(BoxTest, ConstructArrayString_2) {
  string lower[2] = { string("0 <= x"),string("0 <= y") };
  string upper[2] = { string("x <= 1"),string("y <= 1") };

  Box box( lower, upper, 2 );
  EXPECT_EQ( box.dimensions(), 2 );
  EXPECT_EQ( box.getDimensionLowerBound(0), string("0 <= x"));
  EXPECT_EQ( box.getDimensionUpperBound(0), string("x <= 1"));
  EXPECT_EQ( box.getDimensionLowerBound(1), string("0 <= y"));
  EXPECT_EQ( box.getDimensionUpperBound(1), string("y <= 1"));
  EXPECT_EQ( box.getDimensionsBounds(0).first,  string("0 <= x"));
  EXPECT_EQ( box.getDimensionsBounds(0).second, string("x <= 1"));
  EXPECT_EQ( box.getDimensionsBounds(1).first,  string("0 <= y"));
  EXPECT_EQ( box.getDimensionsBounds(1).second, string("y <= 1"));
}

TEST(BoxTest, ConstructArrayPairs_1) {
  pair<string,string> bounds[1] = { pair<string,string>( string("0 <= x"), string("x <= 1") ) };

  Box box( bounds, 1 );
  EXPECT_EQ( box.dimensions(), 1 );
  EXPECT_EQ( box.getDimensionLowerBound(0), string("0 <= x"));
  EXPECT_EQ( box.getDimensionUpperBound(0), string("x <= 1"));
  EXPECT_EQ( box.getDimensionsBounds(0).first,  string("0 <= x"));
  EXPECT_EQ( box.getDimensionsBounds(0).second, string("x <= 1"));
}

TEST(BoxTest, ConstructArrayPairs_2) {
  pair<string,string> bounds[2] = { pair<string,string>( string("0 <= x"), string("x <= 1") ),
                                    pair<string,string>( string("0 <= y"), string("y <= 1") )
                                  };

  Box box( bounds, 2 );
  EXPECT_EQ( box.dimensions(), 2 );
  EXPECT_EQ( box.getDimensionLowerBound(0), string("0 <= x"));
  EXPECT_EQ( box.getDimensionUpperBound(0), string("x <= 1"));
  EXPECT_EQ( box.getDimensionLowerBound(1), string("0 <= y"));
  EXPECT_EQ( box.getDimensionUpperBound(1), string("y <= 1"));
  EXPECT_EQ( box.getDimensionsBounds(0).first,  string("0 <= x"));
  EXPECT_EQ( box.getDimensionsBounds(0).second, string("x <= 1"));
  EXPECT_EQ( box.getDimensionsBounds(1).first,  string("0 <= y"));
  EXPECT_EQ( box.getDimensionsBounds(1).second, string("y <= 1"));
}

TEST(BoxTest, ConstructArrayCharStar_1) {
  char* lower[1] = { (char*) "0 <= x" };
  char* upper[1] = { (char*) "x <= 1" };

  Box box( lower, upper, 1 );
  EXPECT_EQ( box.dimensions(), 1 );
  EXPECT_EQ( box.getDimensionLowerBound(0), string("0 <= x"));
  EXPECT_EQ( box.getDimensionUpperBound(0), string("x <= 1"));
  EXPECT_EQ( box.getDimensionsBounds(0).first,  string("0 <= x"));
  EXPECT_EQ( box.getDimensionsBounds(0).second, string("x <= 1"));
}

TEST(BoxTest, ConstructArrayCharStar_2) {
  char* lower[2] = { (char*) "0 <= x", (char*) "0 <= y" };
  char* upper[2] = { (char*) "x <= 1", (char*) "y <= 1" };

  Box box( lower, upper, 2 );
  EXPECT_EQ( box.dimensions(), 2 );
  EXPECT_EQ( box.getDimensionLowerBound(0), string("0 <= x"));
  EXPECT_EQ( box.getDimensionUpperBound(0), string("x <= 1"));
  EXPECT_EQ( box.getDimensionLowerBound(1), string("0 <= y"));
  EXPECT_EQ( box.getDimensionUpperBound(1), string("y <= 1"));
  EXPECT_EQ( box.getDimensionsBounds(0).first,  string("0 <= x"));
  EXPECT_EQ( box.getDimensionsBounds(0).second, string("x <= 1"));
  EXPECT_EQ( box.getDimensionsBounds(1).first,  string("0 <= y"));
  EXPECT_EQ( box.getDimensionsBounds(1).second, string("y <= 1"));
}
