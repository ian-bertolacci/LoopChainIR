/*! ****************************************************************************
\file RectangularDomain_test.cpp
\authors Ian J. Bertolacci

\brief
To perform unit testsing on the RectangularDomain data structure.

\copyright
Copyright 2015-2016 Colorado State University
*******************************************************************************/

#include "gtest/gtest.h"
#include "RectangularDomain.hpp"
#include <iostream>
#include <utility>
#include <set>

using namespace std;
using namespace LoopChainIR;

/*
Create a simple 1D RectangularDomain on {0..1}
*/
TEST(RectangularDomainTest, Test_Getters_1D ) {
  // Create our lower and upper bounds
  string lower[1] = { "0" };
  string upper[1] = { "1" };
  // There are no symbols in our bounds

  // Construct the domain (lower, upper, dimensions)
  RectangularDomain domain( lower, upper, 1);

  // Test the object
  EXPECT_EQ( domain.dimensions(), 1 );
  EXPECT_EQ( domain.symbolics(), 0 );
  EXPECT_EQ( domain.getLowerBound(0), "0" );
  EXPECT_EQ( domain.getUpperBound(0), "1" );
}

TEST(RectangularDomainTest, Test_Append ) {

  // Create our lower and upper bounds
  string lower =  "0" ;
  string upper =  "1" ;

  RectangularDomain domain(lower, upper);

  RectangularDomain domain2(lower, upper);

  // Test the object
  EXPECT_EQ( domain.dimensions(), 1 );
  EXPECT_EQ( domain.symbolics(), 0 );
  EXPECT_EQ( domain.getLowerBound(0), "0" );
  EXPECT_EQ( domain.getUpperBound(0), "1" );

  RectangularDomain domain3( lower, upper);

  // append
  domain.append(domain2);
  domain.append(domain3);

  // Test the object
  EXPECT_EQ( domain.dimensions(), 3 );
  EXPECT_EQ( domain.symbolics(), 0 );
  EXPECT_EQ( domain.getLowerBound(1), "0" );
  EXPECT_EQ( domain.getUpperBound(1), "1" );


}


TEST(RectangularDomainTest, Test_Append_Symbols ) {

  RectangularDomain* domain;
  {
    // Create our lower and upper bounds
    string lower[1] = { "0" };
    string upper[1] = { "N" };
    // List all symbols (no order required)
    string symbols[1] = { "N" };

    // Construct the domain (lower, upper, dimensions, symbols, # of symbols)
    domain = new RectangularDomain( lower, upper, 1, symbols, 1 );
  }

  {
    // Create our lower and upper bounds
    string lower[1] = { "L" };
    string upper[1] = { "1" };
    // List all symbols (no order required)
    string symbols[1] = { "L" };

    // Construct the domain (lower, upper, dimensions, symbols, # of symbols)
    domain->append( RectangularDomain( lower, upper, 1, symbols, 1 ) );
  }

  // Test the object
  EXPECT_EQ( domain->dimensions(), 2 );
  EXPECT_EQ( domain->symbolics(), 2 );
  EXPECT_EQ( domain->getLowerBound(0), "0" );
  EXPECT_EQ( domain->getUpperBound(0), "N" );
  EXPECT_EQ( domain->getLowerBound(1), "L" );
  EXPECT_EQ( domain->getUpperBound(1), "1" );
  string arr_sym[2] = { "N", "L" };
  set<string> form_symbols( arr_sym, arr_sym+2 );
  EXPECT_EQ( domain->getSymbols(), form_symbols );
}

/*
Create a simple 2D RectangularDomain on {0..1, 0..1}
*/
TEST(RectangularDomainTest, Test_Getters_2D) {
  // Create our lower and upper bounds
  string lower[2] = { "0", "0" };
  string upper[2] = { "1", "1" };
  // There are no symbols in our bounds

  // Construct the domain (lower, upper, dimensions)
  RectangularDomain domain( lower, upper, 2 );

  // Test the object
  EXPECT_EQ( domain.dimensions(), 2 );
  EXPECT_EQ( domain.symbolics(), 0 );
  EXPECT_EQ( domain.getLowerBound(0), "0" );
  EXPECT_EQ( domain.getUpperBound(0), "1" );
  EXPECT_EQ( domain.getLowerBound(1), "0" );
  EXPECT_EQ( domain.getUpperBound(1), "1" );
}

/*
Create a simple 2D RectangularDomain using symbols on {0..0,1..N}
*/
TEST(RectangularDomainTest, Test_Getters_2D_Symbols) {
  // Create our lower and upper bounds
  string lower[2] = { "0", "0" };
  string upper[2] = { "1", "N" };
  // List all symbols (no order required)
  string symbols[1] = { "N" };

  // Construct the domain (lower, upper, dimensions, symbols, # of symbols)
  RectangularDomain domain( lower, upper, 2, symbols, 1 );

  // Test object
  EXPECT_EQ( domain.dimensions(), 2 );
  EXPECT_EQ( domain.symbolics(), 1 );
  EXPECT_EQ( domain.getLowerBound(0), "0" );
  EXPECT_EQ( domain.getUpperBound(0), "1" );
  EXPECT_EQ( domain.getLowerBound(1), "0" );
  EXPECT_EQ( domain.getUpperBound(1), "N" );
  string arr_sym[1] = { "N" };
  set<string> form_symbols( arr_sym, arr_sym+1 );
  EXPECT_EQ( domain.getSymbols(), form_symbols );
}

/*
Create a simple 2D RectangularDomain using symbols on {L..0,1..N}
*/
TEST(RectangularDomainTest, Test_Getters_2D_Symbols_2) {
  // Create our lower and upper bounds
  string lower[2] = { "L", "0" };
  string upper[2] = { "1", "N" };
  // List all symbols (no order required)
  string symbols[2] = { "L", "N" };

  // Construct the domain (lower, upper, dimensions, symbols, # of symbols)
  RectangularDomain domain( lower, upper, 2, symbols, 2 );

  // Test object
  EXPECT_EQ( domain.dimensions(), 2 );
  EXPECT_EQ( domain.symbolics(), 2 );
  EXPECT_EQ( domain.getLowerBound(0), "L" );
  EXPECT_EQ( domain.getUpperBound(0), "1" );
  EXPECT_EQ( domain.getLowerBound(1), "0" );
  EXPECT_EQ( domain.getUpperBound(1), "N" );
  string arr_sym[2] = { "L", "N" };
  set<string> form_symbols( arr_sym, arr_sym+2 );
  EXPECT_EQ( domain.getSymbols(), form_symbols );
}

/*
Create a complex 1D RectangularDomain {12 - 13 + 1 .. 1}
*/
TEST(RectangularDomainTest, Test_Getters_1D_Expressions) {
  // Create our lower and upper bounds
  string lower[1] = { "12 - 13 + 1" };
  string upper[1] = { "1" };
  // There are no symbols in our bounds

  // Construct the domain (lower, upper, dimensions )
  RectangularDomain domain( lower, upper, 1);

  // Test object
  EXPECT_EQ( domain.dimensions(), 1 );
  EXPECT_EQ( domain.symbolics(), 0 );
  EXPECT_EQ( domain.getLowerBound(0), "12 - 13 + 1" );
  EXPECT_EQ( domain.getUpperBound(0), "1" );
}

/*
Create a complex 2D RectangularDomain { 6 + 7 - 15 .. 50 + 42, 3 - 53 .. 12 + 8 }
*/
TEST(RectangularDomainTest, Test_Getters_2D_Expressions) {
  // Create our lower and upper bounds
  string lower[2] = { "6 + 7 - 15", "3 - 53"};
  string upper[2] = { "50 + 42", "12 + 8" };
  // There are no symbols in our bounds

  // Construct the domain (lower, upper, dimensions )
  RectangularDomain domain( lower, upper, 2);

  // Test object
  EXPECT_EQ( domain.dimensions(), 2 );
  EXPECT_EQ( domain.symbolics(), 0 );
  EXPECT_EQ( domain.getLowerBound(0), "6 + 7 - 15" );
  EXPECT_EQ( domain.getUpperBound(0), "50 + 42" );
  EXPECT_EQ( domain.getLowerBound(1), "3 - 53" );
  EXPECT_EQ( domain.getUpperBound(1), "12 + 8" );
}


/*
Create a complex 2D RectangularDomain with symbols { L - 15 .. N * 2, L * 2 .. N * N }
*/
TEST(RectangularDomainTest, Test_Getters_2D_Expressions_Symbols) {
  // Create our lower and upper bounds
  string lower[2] = { "L - 15", "L * 2"};
  string upper[2] = { "N * 2", "N * N" };
  string symbols[2] = { "L", "N" };

  // Construct the domain (lower, upper, dimensions )
  RectangularDomain domain( lower, upper, 2, symbols, 2);

  // Test object
  EXPECT_EQ( domain.dimensions(), 2 );
  EXPECT_EQ( domain.symbolics(), 2 );
  EXPECT_EQ( domain.getLowerBound(0), "L - 15" );
  EXPECT_EQ( domain.getUpperBound(0), "N * 2" );
  EXPECT_EQ( domain.getLowerBound(1), "L * 2" );
  EXPECT_EQ( domain.getUpperBound(1), "N * N" );
  string arr_sym[2] = { "L", "N" };
  set<string> form_symbols( arr_sym, arr_sym+2 );
  EXPECT_EQ( domain.getSymbols(), form_symbols );
}

/*
Create a complex 1D RectangularDomain with symbols { L - 15 .. N * 2, L * 2 .. N * N }
*/
TEST(RectangularDomainTest, Test_Getters_1D_Expressions_Symbols_Set) {
  // Create our lower and upper bounds
  string lower[1] = { "L - 15"};
  string upper[1] = { "N * 2" };
  set<string> symbols;
  symbols.insert("L");
  symbols.insert("N");

  // Construct the domain (lower, upper, dimensions )
  RectangularDomain domain( lower, upper, 1, symbols);

  // Test object
  EXPECT_EQ( domain.dimensions(), 1 );
  EXPECT_EQ( domain.symbolics(), 2 );
  EXPECT_EQ( domain.getLowerBound(0), "L - 15" );
  EXPECT_EQ( domain.getUpperBound(0), "N * 2" );
  EXPECT_EQ( domain.getSymbols(), symbols );
}

/*
Create a complex 2D RectangularDomain with symbols { L - 15 .. N * 2, L * 2 .. N * N }
*/
TEST(RectangularDomainTest, Test_Getters_2D_Expressions_Symbols_Set) {
  // Create our lower and upper bounds
  string lower[2] = { "L - 15", "L * 2"};
  string upper[2] = { "N * 2", "N * N" };
  set<string> symbols;
  symbols.insert("L");
  symbols.insert("N");

  // Construct the domain (lower, upper, dimensions )
  RectangularDomain domain( lower, upper, 2, symbols);

  // Test object
  EXPECT_EQ( domain.dimensions(), 2 );
  EXPECT_EQ( domain.symbolics(), 2 );
  EXPECT_EQ( domain.getLowerBound(0), "L - 15" );
  EXPECT_EQ( domain.getUpperBound(0), "N * 2" );
  EXPECT_EQ( domain.getLowerBound(1), "L * 2" );
  EXPECT_EQ( domain.getUpperBound(1), "N * N" );
  EXPECT_EQ( domain.getSymbols(), symbols );
}
