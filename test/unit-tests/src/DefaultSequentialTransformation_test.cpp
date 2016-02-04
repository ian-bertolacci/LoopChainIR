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

TEST(DefaultSequentialTransformationTest, GEN_1N_1D_Empty_Loop) {
  LoopChain chain;

  {
    string lower[1] = { "1" };
    string upper[1] = { "0" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1 ) ) );
  }

  vector<Transformation*> schedulers;
  schedulers.push_back( new DefaultSequentialTransformation() );

  Schedule sched( chain );
  apply( schedulers );
  
  ASSERT_EQ( sched.codegen(), string("{\n}\n") );
}

TEST(DefaultSequentialTransformationTest, GEN_1N_1D) {
  LoopChain chain;

  {
    string lower[1] = { "1" };
    string upper[1] = { "N" };
    string symbols[1] = { "N" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1, symbols, 1 ) ) );
  }

  vector<Transformation*> schedulers;
  schedulers.push_back( new DefaultSequentialTransformation() );

  Schedule sched( chain );
  sched.apply( chain, schedulers );

  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}

TEST(DefaultSequentialTransformationTest, GEN_1N_2D) {
  LoopChain chain;

  {
    string lower[2] = { "0", "K" };
    string upper[2] = { "N", "M" };
    string symbol[3] = { "K", "N", "M" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbol, 3 ) ) );
  }

  vector<Transformation*> schedulers;
  schedulers.push_back( new DefaultSequentialTransformation() );

  Schedule sched( chain );
  sched.apply( schedulers );

  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}

TEST(DefaultSequentialTransformationTest, GEN_4N_1D) {
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

  vector<Transformation*> schedulers;
  schedulers.push_back( new DefaultSequentialTransformation() );

  Schedule sched( chain )
  sched.apply( chain, schedulers );

  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}

TEST(DefaultSequentialTransformationTest, GEN_3N_1D_2D_3D) {
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

  vector<Transformation*> schedulers;
  schedulers.push_back( new DefaultSequentialTransformation() );

  Schedule sched( chain );
  apply( schedulers );

  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}


TEST(DefaultSequentialTransformationTest, SC_Loops_Default) {
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

  vector<Transformation*> schedulers;
  schedulers.push_back( new DefaultSequentialTransformation() );

  Schedule sched( chain );
  apply( schedulers );

  ASSERT_NE( sched.codegen(), string("{\n}\n") );
}
