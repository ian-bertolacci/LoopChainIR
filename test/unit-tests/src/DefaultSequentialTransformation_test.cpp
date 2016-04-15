/*! ****************************************************************************
\file DefaultSequentialTransformation_test.cpp
\autors Ian J. Bertolacci

\brief
To perform unit testsing on the DefaultSequentialTransformation code generator.

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/

#include "gtest/gtest.h"
#include "DefaultSequentialTransformation.hpp"
#include <iostream>
#include <utility>

using namespace std;
using namespace LoopChainIR;

/*
Apply DefaultSequentialTransformation to a loop with no iterations
*/
TEST(DefaultSequentialTransformationTest, GEN_1N_1D_Empty_Loop) {
  LoopChain chain;

  {
    // This loop is empty, since its lower bound is greater than
    // its upper bound. It is still valid, however.
    string lower[1] = { "1" };
    string upper[1] = { "0" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1 ) ) );
  }

  // Create an ordered list of Transformations
  vector<Transformation*> schedulers;
  // Append the Default Sequential transformation to the list. (It's first)
  schedulers.push_back( new DefaultSequentialTransformation() );

  // Create Schedule object from chain
  Schedule sched( chain );
  // Apply list of transformations
  // In this example, only the DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  // Because this is an empty loop, the code will be an empty statement body
  ASSERT_EQ( sched.codegen(), string("{\n}\n") );
}

/*
Apply DefaultSequentialTransformation to 1N_1D loop
*/
TEST(DefaultSequentialTransformationTest, GEN_1N_1D) {
  // Create loop chain
  LoopChain chain;

  {
    string lower[1] = { "1" };
    string upper[1] = { "N" };
    string symbols[1] = { "N" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbols, 1 ) ) );
  }

  // Create list with DefaultSequentialTransformation as only entry
  vector<Transformation*> schedulers;
  schedulers.push_back( new DefaultSequentialTransformation() );

  // Create schedule from loop chain
  Schedule sched( chain );

  // Apply DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  // In this case, the loop chain is not empty.
  // However, we can really only check to see if the code body is also not empty
  // Thus this is an assert not equals to empty statement body
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}

/*
Apply DefaultSequentialTransformation on a 1N_2D loop
*/
TEST(DefaultSequentialTransformationTest, GEN_1N_2D) {
  // Create loop chain
  LoopChain chain;

  {
    string lower[2] = { "0", "K" };
    string upper[2] = { "N", "M" };
    string symbol[3] = { "K", "N", "M" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbol, 3 ) ) );
  }

  // Create list with DefaultSequentialTransformation as only entry
  vector<Transformation*> schedulers;
  schedulers.push_back( new DefaultSequentialTransformation() );

  // Create schedule from loop chain
  Schedule sched( chain );

  // Apply DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}

/*
Apply DefaultSequentialTransformation to a 4N_1D loop chain
*/
TEST(DefaultSequentialTransformationTest, GEN_4N_1D) {
  // Create loop chain
  LoopChain chain;

  {
    string lower[1] = { "1" };
    string upper[1] = { "N" };
    string symbol[1] = { "N" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbol, 1 ) ) );
  }

  {
    string lower[1] = { "1" };
    string upper[1] = { "M" };
    string symbol[1] = { "M" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbol, 1 ) ) );
  }

  {
    string lower[1] = { "1" };
    string upper[1] = { "K" };
    string symbol[1] = { "K" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbol, 1 ) ) );
  }

  {
    string lower[1] = { "1" };
    string upper[1] = { "J" };
    string symbol[1] = { "J" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbol, 1 ) ) );
  }

  // Create list with DefaultSequentialTransformation as only entry
  vector<Transformation*> schedulers;
  schedulers.push_back( new DefaultSequentialTransformation() );

  // Create schedule from loop chain
  Schedule sched( chain );

  // Apply DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}

/*

*/
TEST(DefaultSequentialTransformationTest, GEN_3N_1D_2D_3D) {
  // Create loop chain
  LoopChain chain;

  {
    string lower[1] = {"0"};
    string upper[1] = {"N"};
    string symbols[1] = {"N"};

    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbols, 1 ) ) );
  }

  {
    string lower[2] = {"1","2"};
    string upper[2] = {"M","M"};
    string symbols[2] = {"M"};

    chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbols, 1 ) ) );
  }

  {
    string lower[3] = {"3","4","5"};
    string upper[3] = {"O","P","6"};
    string symbols[3] = {"O","P"};

    chain.append( LoopNest( RectangularDomain( lower, upper, 3, symbols, 2 ) ) );
  }

  // Create list with DefaultSequentialTransformation as only entry
  vector<Transformation*> schedulers;
  schedulers.push_back( new DefaultSequentialTransformation() );

  // Create schedule from loop chain
  Schedule sched( chain );

  // Apply DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}

/*
Apply DefaultSequentialTransformation to SuperComputing loops (4N_3D)
*/
TEST(DefaultSequentialTransformationTest, SC_Loops_Default) {
  // Create loop chain
  LoopChain chain;

  string symbols[6] = { "Lx","Ux","Ly","Uy","Lz","Uz" };
  string lower[3] = { "Lx", "Ly", "Lz" };

  {
    string upper[3] = { "Ux+1", "Uy", "Uz" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 3, symbols, 6 ) ) );
  }

  {
    string upper[3] = { "Ux", "Uy+1", "Uz" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 3, symbols, 6 ) ) );
  }

  {
    string upper[3] = { "Ux", "Uy", "Uz+1" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 3, symbols, 6 ) ) );
  }

  {
    string upper[3] = { "Ux", "Uy", "Uz" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 3, symbols, 6 ) ) );
  }

  // Create list with DefaultSequentialTransformation as only entry
  vector<Transformation*> schedulers;
  schedulers.push_back( new DefaultSequentialTransformation() );

  // Create schedule from loop chain
  Schedule sched( chain );

  // Apply DefaultSequentialTransformation
  sched.apply( schedulers );

  // Test
  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}
