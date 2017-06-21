/*! ****************************************************************************
\file LoopNest_test.cpp
\authors Ian J. Bertolacci

\brief
To perform unit testsing on the LoopNest data structure.

\copyright
Copyright 2015-2016 Colorado State University
*******************************************************************************/

#include "gtest/gtest.h"
#include <LoopChainIR/LoopNest.hpp>
#include <iostream>
#include <utility>

using namespace std;
using namespace LoopChainIR;

/*
Create a simple 1D Loop nest
*/
TEST(LoopNestTest, Test_Getters_1D) {
  // Construct Domain
  string lower[1] = { "0" };
  string upper[1] = { "1" };

  RectangularDomain domain( lower, upper, 1 );

  // Create LoopNest from domain
  LoopNest nest( domain );

  // Test
  RectangularDomain got_domain = nest.getDomain();

  // Compare domains
  EXPECT_EQ( got_domain.dimensions(), domain.dimensions() );
  EXPECT_EQ( got_domain.symbolics(), domain.symbolics() );

  for( RectangularDomain::size_type d = 0; d < got_domain.dimensions(); d += 1){
    EXPECT_EQ( got_domain.getLowerBound(d), domain.getLowerBound(d) );
    EXPECT_EQ( got_domain.getUpperBound(d), domain.getUpperBound(d) );
  }

  EXPECT_EQ( got_domain.getSymbols(), domain.getSymbols() );
}

/*
Create a simple 1D Loop with symbols
*/
TEST(LoopNestTest, Test_Getters_1D_Symbols) {
  // Construct domain
  string lower[1] = { "0" };
  string upper[1] = { "N" };
  string symbols[1] = { "N" };

  RectangularDomain domain( lower, upper, 1, symbols, 1 );
  // COnstruct LoopNest from domain
  LoopNest nest( domain );

  LoopNest otherNest = nest;

  // Test
  RectangularDomain got_domain = nest.getDomain();

  // Compare domains
  EXPECT_EQ( got_domain.dimensions(), domain.dimensions() );
  EXPECT_EQ( got_domain.symbolics(), domain.symbolics() );
  EXPECT_EQ( otherNest.getDomain().dimensions(), domain.dimensions() );
  EXPECT_EQ( otherNest.getDomain().symbolics(), domain.symbolics() );

  for( RectangularDomain::size_type d = 0; d < got_domain.dimensions(); d += 1){
    EXPECT_EQ( got_domain.getLowerBound(d), domain.getLowerBound(d) );
    EXPECT_EQ( got_domain.getUpperBound(d), domain.getUpperBound(d) );
  }

  EXPECT_EQ( got_domain.getSymbols(), domain.getSymbols() );
}

TEST( LoopNestTest, construct_dataspaces ){
  // COnstruct LoopNest from domain
  LoopNest nest(
      RectangularDomain( { make_pair( "0", "N") }, {"N" } ),
      {
        Dataspace(  "A",
                    TupleCollection( {
                      Tuple( {  0, -1 } ),
                      Tuple( { -1,  0 } ),
                      Tuple( {  0,  0 } ),
                      Tuple( {  0,  1 } ),
                      Tuple( {  1,  0 } )
                    } ),
                    TupleCollection( {
                      Tuple( {  0,  0 } )
                    } )
                ),
        Dataspace(  "B",
                    TupleCollection( {
                      Tuple( {  0, -1 } ),
                      Tuple( { -1,  0 } ),
                      Tuple( {  0,  0 } ),
                      Tuple( {  0,  1 } ),
                      Tuple( {  1,  0 } )
                    } ),
                    TupleCollection( {
                      Tuple( {  0,  0 } )
                    } )
                )
      }
  );
}

TEST( LoopNestTest, shift_dataspaces ){
  // COnstruct LoopNest from domain
  LoopNest nest(
      RectangularDomain( { make_pair( "0", "N") }, {"N" } ),
      {
        Dataspace(  "A",
                    TupleCollection( {
                      Tuple( {  0,  0 } ),
                      Tuple( {  1, -1 } )
                    } ),
                    TupleCollection( 2 )
                ),
        Dataspace(  "B",
                    TupleCollection( 2 ),
                    TupleCollection( {
                      Tuple( {  0,  0 } ),
                      Tuple( {  1, -1 } )
                    } )
                )
      }
  );

  nest.shiftDataspaces( Tuple( {1,1} ) );

  /*
  for( Dataspace& dataspace : nest.getDataspaces() ){
    cout << dataspace << endl;
  }
  */
}
