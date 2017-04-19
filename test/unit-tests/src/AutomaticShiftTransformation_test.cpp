/*! ****************************************************************************
\file AutomaticShiftTransformation_test.cpp
\authors Ian J. Bertolacci

\brief
To perform unit testsing on the ShiftTransformation code generator.

\copyright
Copyright 2015-2016 Colorado State University
*******************************************************************************/

#include "gtest/gtest.h"
#include "AutomaticShiftTransformation.hpp"
#include <iostream>
#include <utility>

using namespace std;
using namespace LoopChainIR;

TEST( DataSpaceMinMax_test, construct ){
  Dataspace dataspace_b(  "B",
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
                      );
  ASSERT_NO_THROW({ DataspaceMinMax min_max( dataspace_b ); });
}

TEST( DataSpaceMinMax_test, min_max ){
  Dataspace dataspace_b(  "B",
                          // reads
                          TupleCollection( {
                            Tuple( {  0, -1 } ),
                            Tuple( { -2,  0 } ),
                            Tuple( {  0,  0 } ),
                            Tuple( {  0, 3 } ),
                            Tuple( {  4,  0 } )
                          } ),
                          // writes
                          TupleCollection( {
                            Tuple( {  0, -5 } ),
                            Tuple( { -6,  0 } ),
                            Tuple( {  0,  0 } ),
                            Tuple( {  0,  7 } ),
                            Tuple( {  8,  0 } )
                          } )
                      );
  DataspaceMinMax min_max( dataspace_b );
  EXPECT_EQ( min_max.minRead(),  Tuple( { -2, -1 } ) );
  EXPECT_EQ( min_max.maxRead(),  Tuple( {  4,  3 } ) );
  EXPECT_EQ( min_max.minWrite(), Tuple( { -6, -5 } ) );
  EXPECT_EQ( min_max.maxWrite(), Tuple( {  8,  7 } ) );
}

TEST( DataSpaceMinMax_test, postShiftUpdate ){
  Dataspace dataspace_b(  "B",
                          // reads
                          TupleCollection( {
                            Tuple( {  0, -1 } ),
                            Tuple( { -2,  0 } ),
                            Tuple( {  0,  0 } ),
                            Tuple( {  0, 3 } ),
                            Tuple( {  4,  0 } )
                          } ),
                          // writes
                          TupleCollection( {
                            Tuple( {  0, -5 } ),
                            Tuple( { -6,  0 } ),
                            Tuple( {  0,  0 } ),
                            Tuple( {  0,  7 } ),
                            Tuple( {  8,  0 } )
                          } )
                      );

  DataspaceMinMax min_max( dataspace_b );
  EXPECT_EQ( min_max.minRead(),  Tuple( { -2, -1 } ) );
  EXPECT_EQ( min_max.maxRead(),  Tuple( {  4,  3 } ) );
  EXPECT_EQ( min_max.minWrite(), Tuple( { -6, -5 } ) );
  EXPECT_EQ( min_max.maxWrite(), Tuple( {  8,  7 } ) );

  min_max.postShiftUpdate( Tuple( { -1, 1 } ) );
  EXPECT_EQ( min_max.minRead(),  Tuple( { -1, -2 } ) );
  EXPECT_EQ( min_max.maxRead(),  Tuple( {  5,  2 } ) );
  EXPECT_EQ( min_max.minWrite(), Tuple( { -5, -6 } ) );
  EXPECT_EQ( min_max.maxWrite(), Tuple( {  9,  6 } ) );
}

TEST( DataSpaceMinMax_test, selfUnion ){
  Dataspace dataspace_a(  "A",
                          // reads
                          TupleCollection( {
                            Tuple( {  0, -1 } ),
                            Tuple( { -2,  0 } )
                          } ),
                          // writes
                          TupleCollection( {
                            Tuple( {  0, -4 } ),
                            Tuple( { -5,  0 } )
                          } )
                      );

  Dataspace dataspace_a2(  "A",
                          // reads
                          TupleCollection( {
                            Tuple( {  1, -1 } ),
                            Tuple( { -2,  2 } )
                          } ),
                          // writes
                          TupleCollection( {
                            Tuple( {  3, -4 } ),
                            Tuple( { -5,  4 } )
                          } )
                      );
  Dataspace dataspace_b(  "B",
                          // reads
                          TupleCollection( {
                            Tuple( {  100, -100 } ),
                            Tuple( { -200,  200 } )
                          } ),
                          // writes
                          TupleCollection( {
                            Tuple( {  300, -400 } ),
                            Tuple( { -500,  400 } )
                          } )
                      );

  DataspaceMinMax min_max( dataspace_a );
  EXPECT_EQ( min_max.minRead(),  Tuple( { -2, -1 } ) );
  EXPECT_EQ( min_max.maxRead(),  Tuple( {  0,  0 } ) );
  EXPECT_EQ( min_max.minWrite(), Tuple( { -5, -4 } ) );
  EXPECT_EQ( min_max.maxWrite(), Tuple( {  0,  0 } ) );
  min_max.selfUnion( dataspace_a2 );
  EXPECT_EQ( min_max.minRead(),  Tuple( { -2, -1 } ) );
  EXPECT_EQ( min_max.maxRead(),  Tuple( {  1,  2 } ) );
  EXPECT_EQ( min_max.minWrite(), Tuple( { -5, -4 } ) );
  EXPECT_EQ( min_max.maxWrite(), Tuple( {  3,  4 } ) );
  min_max.selfUnion( dataspace_b );
  EXPECT_EQ( min_max.minRead(),  Tuple( { -2, -1 } ) );
  EXPECT_EQ( min_max.maxRead(),  Tuple( {  1,  2 } ) );
  EXPECT_EQ( min_max.minWrite(), Tuple( { -5, -4 } ) );
  EXPECT_EQ( min_max.maxWrite(), Tuple( {  3,  4 } ) );
}

TEST( DataspaceMinMax_test, construct_empty ){
  Dataspace dataspace_a(  "A",
                          // reads
                          TupleCollection( 2 ),
                          // writes
                          TupleCollection( 2 )
                      );

  DataspaceMinMax min_max( dataspace_a );

  EXPECT_EQ( min_max.maxRead(), Tuple::createMagicEmptyTuple() );
  EXPECT_EQ( min_max.minRead(), Tuple::createMagicEmptyTuple() );
  EXPECT_EQ( min_max.maxWrite(), Tuple::createMagicEmptyTuple() );
  EXPECT_EQ( min_max.minWrite(), Tuple::createMagicEmptyTuple() );
}

TEST( DataspaceMinMax_test, postShiftUpdate_empty ){
  Dataspace dataspace_a(  "A",
                          // reads
                          TupleCollection( 2 ),
                          // writes
                          TupleCollection( 2 )
                      );

  DataspaceMinMax min_max( dataspace_a );
  min_max.postShiftUpdate( Tuple( {0,1} ) );
  EXPECT_EQ( min_max.maxRead(), Tuple::createMagicEmptyTuple() );
  EXPECT_EQ( min_max.minRead(), Tuple::createMagicEmptyTuple() );
  EXPECT_EQ( min_max.maxWrite(), Tuple::createMagicEmptyTuple() );
  EXPECT_EQ( min_max.minWrite(), Tuple::createMagicEmptyTuple() );
}
TEST( DataspaceMinMax_test, selfUnion_empty ){
  Dataspace dataspace_a(  "A",
                          // reads
                          TupleCollection( 2 ),
                          // writes
                          TupleCollection( 2 )
                      );


  DataspaceMinMax min_max( dataspace_a );

  EXPECT_EQ( min_max.maxRead(), Tuple::createMagicEmptyTuple() );
  EXPECT_EQ( min_max.minRead(), Tuple::createMagicEmptyTuple() );
  EXPECT_EQ( min_max.maxWrite(), Tuple::createMagicEmptyTuple() );
  EXPECT_EQ( min_max.minWrite(), Tuple::createMagicEmptyTuple() );

  Dataspace dataspace_a2(  "A",
                          // reads
                          TupleCollection( { Tuple( {  1, -1 } ) } ),
                          // writes
                          TupleCollection( 2 )
                        );

  min_max.selfUnion( dataspace_a2 );

  EXPECT_EQ( min_max.maxRead(), Tuple( {  1, -1 } ) );
  EXPECT_EQ( min_max.minRead(), Tuple( {  1, -1 } ) );
  EXPECT_EQ( min_max.maxWrite(), Tuple::createMagicEmptyTuple() );
  EXPECT_EQ( min_max.minWrite(), Tuple::createMagicEmptyTuple() );

  Dataspace dataspace_a3(  "A",
                          // reads
                          TupleCollection( 2 ),
                          // writes
                          TupleCollection( { Tuple( {  2, -2 } ) } )
                        );

  min_max.selfUnion( dataspace_a3 );

  EXPECT_EQ( min_max.maxRead(), Tuple( {  1, -1 } ) );
  EXPECT_EQ( min_max.minRead(), Tuple( {  1, -1 } ) );
  EXPECT_EQ( min_max.maxWrite(), Tuple( {  2, -2 } ) );
  EXPECT_EQ( min_max.minWrite(), Tuple( {  2, -2 } ) );


}

TEST( DataspaceMinMaxCollection_test, construct ){
  Dataspace dataspace_a(  "A",
                          // reads
                          TupleCollection( {
                            Tuple( {  0, -1 } ),
                            Tuple( { -2,  0 } )
                          } ),
                          // writes
                          TupleCollection( {
                            Tuple( {  0, -4 } ),
                            Tuple( { -5,  0 } )
                          } )
                      );

  Dataspace dataspace_b(  "B",
                          // reads
                          TupleCollection( {
                            Tuple( {  100, -100 } ),
                            Tuple( { -200,  200 } )
                          } ),
                          // writes
                          TupleCollection( {
                            Tuple( {  300, -400 } ),
                            Tuple( { -500,  400 } )
                          } )
                      );
  ASSERT_NO_THROW({ DataspaceMinMaxCollection maythrow( std::list<Dataspace>( {dataspace_a, dataspace_b} ) ); });

  DataspaceMinMaxCollection collection( std::list<Dataspace>( {dataspace_a, dataspace_b} ) );
}

TEST( DataspaceMinMaxCollection_test, selfUnion ){
  Dataspace dataspace_a(  "A",
                          // reads
                          TupleCollection( {
                            Tuple( {  0, -1 } ),
                            Tuple( { -2,  0 } )
                          } ),
                          // writes
                          TupleCollection( {
                            Tuple( {  0, -4 } ),
                            Tuple( { -5,  0 } )
                          } )
                      );

  Dataspace dataspace_a2(  "A",
                          // reads
                          TupleCollection( {
                            Tuple( {  1, -1 } ),
                            Tuple( { -2,  2 } )
                          } ),
                          // writes
                          TupleCollection( {
                            Tuple( {  3, -4 } ),
                            Tuple( { -5,  4 } )
                          } )
                      );
  Dataspace dataspace_b(  "B",
                          // reads
                          TupleCollection( {
                            Tuple( {  100, -100 } ),
                            Tuple( { -200,  200 } )
                          } ),
                          // writes
                          TupleCollection( {
                            Tuple( {  300, -400 } ),
                            Tuple( { -500,  400 } )
                          } )
                      );

  DataspaceMinMaxCollection collection( std::list<Dataspace>( {dataspace_a, dataspace_b} ) );

  EXPECT_EQ( collection["A"].minRead(),  Tuple( { -2, -1 } ) );
  EXPECT_EQ( collection["A"].maxRead(),  Tuple( {  0,  0 } ) );
  EXPECT_EQ( collection["A"].minWrite(), Tuple( { -5, -4 } ) );
  EXPECT_EQ( collection["A"].maxWrite(), Tuple( {  0,  0 } ) );

  EXPECT_EQ( collection["B"].minRead(),  Tuple( { -200, -100 } ) );
  EXPECT_EQ( collection["B"].maxRead(),  Tuple( {  100,  200 } ) );
  EXPECT_EQ( collection["B"].minWrite(), Tuple( { -500, -400 } ) );
  EXPECT_EQ( collection["B"].maxWrite(), Tuple( {  300,  400 } ) );


  collection.selfUnion( dataspace_a2 );

  EXPECT_EQ( collection["A"].minRead(),  Tuple( { -2, -1 } ) );
  EXPECT_EQ( collection["A"].maxRead(),  Tuple( {  1,  2 } ) );
  EXPECT_EQ( collection["A"].minWrite(), Tuple( { -5, -4 } ) );
  EXPECT_EQ( collection["A"].maxWrite(), Tuple( {  3,  4 } ) );

  EXPECT_EQ( collection["B"].minRead(),  Tuple( { -200, -100 } ) );
  EXPECT_EQ( collection["B"].maxRead(),  Tuple( {  100,  200 } ) );
  EXPECT_EQ( collection["B"].minWrite(), Tuple( { -500, -400 } ) );
  EXPECT_EQ( collection["B"].maxWrite(), Tuple( {  300,  400 } ) );
}


TEST( DataspaceMinMaxCollection_test, postShiftUpdate ){
  Dataspace dataspace_a(  "A",
                          // reads
                          TupleCollection( {
                            Tuple( {  0, -1 } ),
                            Tuple( { -2,  0 } )
                          } ),
                          // writes
                          TupleCollection( {
                            Tuple( {  0, -4 } ),
                            Tuple( { -5,  0 } )
                          } )
                      );

  Dataspace dataspace_a2(  "A",
                          // reads
                          TupleCollection( {
                            Tuple( {  1, -1 } ),
                            Tuple( { -2,  2 } )
                          } ),
                          // writes
                          TupleCollection( {
                            Tuple( {  3, -4 } ),
                            Tuple( { -5,  4 } )
                          } )
                      );
  Dataspace dataspace_b(  "B",
                          // reads
                          TupleCollection( {
                            Tuple( {  100, -100 } ),
                            Tuple( { -200,  200 } )
                          } ),
                          // writes
                          TupleCollection( {
                            Tuple( {  300, -400 } ),
                            Tuple( { -500,  400 } )
                          } )
                      );

  DataspaceMinMaxCollection collection( std::list<Dataspace>( {dataspace_a, dataspace_b} ) );

  EXPECT_EQ( collection["A"].minRead(),  Tuple( { -2, -1 } ) );
  EXPECT_EQ( collection["A"].maxRead(),  Tuple( {  0,  0 } ) );
  EXPECT_EQ( collection["A"].minWrite(), Tuple( { -5, -4 } ) );
  EXPECT_EQ( collection["A"].maxWrite(), Tuple( {  0,  0 } ) );

  EXPECT_EQ( collection["B"].minRead(),  Tuple( { -200, -100 } ) );
  EXPECT_EQ( collection["B"].maxRead(),  Tuple( {  100,  200 } ) );
  EXPECT_EQ( collection["B"].minWrite(), Tuple( { -500, -400 } ) );
  EXPECT_EQ( collection["B"].maxWrite(), Tuple( {  300,  400 } ) );


  collection.postShiftUpdate( Tuple( { -1, 1 } ) );

  EXPECT_EQ( collection["A"].minRead(),  Tuple( { -1, -2 } ) );
  EXPECT_EQ( collection["A"].maxRead(),  Tuple( {  1, -1 } ) );
  EXPECT_EQ( collection["A"].minWrite(), Tuple( { -4, -5 } ) );
  EXPECT_EQ( collection["A"].maxWrite(), Tuple( {  1, -1 } ) );

  EXPECT_EQ( collection["B"].minRead(),  Tuple( { -199, -101 } ) );
  EXPECT_EQ( collection["B"].maxRead(),  Tuple( {  101,  199 } ) );
  EXPECT_EQ( collection["B"].minWrite(), Tuple( { -499, -401 } ) );
  EXPECT_EQ( collection["B"].maxWrite(), Tuple( {  301,  399 } ) );
}

TEST( computeShiftTuplesForFusion_test, no_dataspaces ){

  LoopChain chain;
  {
    string lower[1] = { "1" };
    string upper[1] = { "N" };
    chain.append( LoopNest( RectangularDomain( lower, upper, 1 ) ) );
  }

  list<Tuple> test = AutomaticShiftTransformation::computeShiftTuplesForFusion( chain );
  list<Tuple> actual;

  ASSERT_EQ( test.size(), actual.size() );
}

TEST( computeShiftTuplesForFusion_test, unrolled_jacobi_1D ){

  LoopChain chain;
  string lower[1] = { "1" };
  string upper[1] = { "N" };

  chain.append(
    LoopNest(
      RectangularDomain( lower, upper, 1 ),
      {
        Dataspace(
          "B",
          // Reads
          TupleCollection({
            Tuple({ -1 }),
            Tuple({  0 }),
            Tuple({  1 }),
          }),
          // writes
          TupleCollection( 1 )
        ),
        Dataspace(
          "A",
          // Reads
          TupleCollection( 1 ),
          // writes
          TupleCollection({
            Tuple({ 0 })
          })
        )
      }
    )
  );

  chain.append(
    LoopNest(
      RectangularDomain( lower, upper, 1 ),
      {
        Dataspace(
          "A",
          // Reads
          TupleCollection({
            Tuple({ -1 }),
            Tuple({  0 }),
            Tuple({  1 }),
          }),
          // writes
          TupleCollection( 1 )
        ),
        Dataspace(
          "B",
          // Reads
          TupleCollection( 1 ),
          // writes
          TupleCollection({
            Tuple({ 0 })
          })
        )
      }
    )
  );

  list<Tuple> test = AutomaticShiftTransformation::computeShiftTuplesForFusion( chain );
  list<Tuple> actual = { Tuple({0}), Tuple({1}) };

  ASSERT_EQ( test.size(), actual.size() );
  for( list<Tuple>::iterator test_it = test.begin(), actual_it = actual.begin();
       test_it != test.end() && actual_it != actual.end();
       ++test_it, ++actual_it ){
    ASSERT_EQ( *test_it, *actual_it );
  }
}

TEST( computeShiftTuplesForFusion_test, unrolled_jacobi_2D ){

  LoopChain chain;
  string lower[2] = { "1", "1" };
  string upper[2] = { "N", "N" };

  chain.append(
    LoopNest(
      RectangularDomain( lower, upper, 2 ),
      {
        Dataspace(
          "B",
          // Reads
          TupleCollection({
            Tuple({ -1,  0 }),
            Tuple({  0, -1 }),
            Tuple({  0,  0 }),
            Tuple({  1,  0 }),
            Tuple({  0,  1 }),
          }),
          // writes
          TupleCollection( 2 )
        ),
        Dataspace(
          "A",
          // Reads
          TupleCollection( 2 ),
          // writes
          TupleCollection({
            Tuple({ 0, 0 })
          })
        )
      }
    )
  );

  chain.append(
    LoopNest(
      RectangularDomain( lower, upper, 2 ),
      {
        Dataspace(
          "A",
          // Reads
          TupleCollection({
            Tuple({ -1,  0 }),
            Tuple({  0, -1 }),
            Tuple({  0,  0 }),
            Tuple({  1,  0 }),
            Tuple({  0,  1 }),
          }),
          // writes
          TupleCollection( 2 )
        ),
        Dataspace(
          "B",
          // Reads
          TupleCollection( 2 ),
          // writes
          TupleCollection({
            Tuple({ 0, 0 })
          })
        )
      }
    )
  );

  list<Tuple> test = AutomaticShiftTransformation::computeShiftTuplesForFusion( chain );
  list<Tuple> actual = { Tuple({0, 0}), Tuple({1, 1}) };

  ASSERT_EQ( test.size(), actual.size() );
  for( list<Tuple>::iterator test_it = test.begin(), actual_it = actual.begin();
       test_it != test.end() && actual_it != actual.end();
       ++test_it, ++actual_it ){
    ASSERT_EQ( *test_it, *actual_it );
  }
}


TEST( computeShiftForFusion_test, unrolled_jacobi_2D_with_transforms ){

  LoopChain chain;
  string lower[2] = { "1", "1" };
  string upper[2] = { "N", "N" };

  chain.append(
    LoopNest(
      RectangularDomain( lower, upper, 2 ),
      {
        Dataspace(
          "B",
          // Reads
          TupleCollection({
            Tuple({ -1,  0 }),
            Tuple({  0, -1 }),
            Tuple({  0,  0 }),
            Tuple({  1,  0 }),
            Tuple({  0,  1 }),
          }),
          // writes
          TupleCollection( 2 )
        ),
        Dataspace(
          "A",
          // Reads
          TupleCollection( 2 ),
          // writes
          TupleCollection({
            Tuple({ 0, 0 })
          })
        )
      }
    )
  );

  chain.append(
    LoopNest(
      RectangularDomain( lower, upper, 2 ),
      {
        Dataspace(
          "A",
          // Reads
          TupleCollection({
            Tuple({ -1,  0 }),
            Tuple({  0, -1 }),
            Tuple({  0,  0 }),
            Tuple({  1,  0 }),
            Tuple({  0,  1 }),
          }),
          // writes
          TupleCollection( 2 )
        ),
        Dataspace(
          "B",
          // Reads
          TupleCollection( 2 ),
          // writes
          TupleCollection({
            Tuple({ 0, 0 })
          })
        )
      }
    )
  );

  ASSERT_NO_THROW({
    list<ShiftTransformation*> test = AutomaticShiftTransformation::computeShiftForFusion( chain );
  });

  list<ShiftTransformation*> test = AutomaticShiftTransformation::computeShiftForFusion( chain );

  // actually writing a test for this is burdensom
  /*
  for( list<ShiftTransformation*>::iterator test_it = test.begin();
       test_it != test.end();
       ++test_it){
    cout << "( ";
    for( string extent : (*test_it)->getExtents() ){
      cout << extent << " ";
    }
    cout << ")" << endl;
  }
  */
}
