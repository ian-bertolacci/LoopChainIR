/*! ****************************************************************************
\file Accesses_test.cpp
\authors Ian J. Bertolacci

\brief
To perform unit testsing on the Tuple, TupleCollection, and DataSpace classes.

\copyright
Copyright 2016 University of Arizona
*******************************************************************************/

#include "gtest/gtest.h"
#include <LoopChainIR/Accesses.hpp>
#include <LoopChainIR/util.hpp>
#include <iostream>
#include <utility>
#include <limits>

using namespace std;
using namespace LoopChainIR;

TEST( Tuple_test, construct ){
  vector<int> values = { 1, 2, 3 };
  ASSERT_NO_THROW({ Tuple maythrow( values );  });
  Tuple tuple( values );
  ASSERT_EQ( tuple.dimensions(), values.size() );
  for( Tuple::size_type d = 0; d < tuple.dimensions(); d += 1 ){
    ASSERT_EQ( tuple[d], values[d] );
  }
}


TEST( Tuple_test, construct_str ){
  vector<string> values = { "1", "2", "3" };
  vector<int> int_values = { 1, 2, 3 };
  ASSERT_NO_THROW({ Tuple maythrow( values );  });
  Tuple tuple( values );
  ASSERT_EQ( tuple.dimensions(), values.size() );
  for( Tuple::size_type d = 0; d < tuple.dimensions(); d += 1 ){
    ASSERT_EQ( tuple[d], int_values[d] );
  }
}

TEST( Tuple_test, construct_str_throw ){
  vector<string> values = { "1", "N", "3" };
  ASSERT_THROW({ Tuple maythrow( values ); }, std::invalid_argument );
}

TEST( Tuple_test, less_than ){
  EXPECT_FALSE( Tuple( { 0, 0, 0 } ) < Tuple( { 0, 0, 0 } ) );
  EXPECT_TRUE(  Tuple( { 0, 0    } ) < Tuple( { 0, 0, 0 } ) );
  EXPECT_FALSE( Tuple( { 0, 0, 0 } ) < Tuple( { 0, 0    } ) );

  EXPECT_TRUE(  Tuple( { 0, 0, 0 } ) < Tuple( { 0, 0, 1 } ) );
  EXPECT_TRUE(  Tuple( { 0, 0, 0 } ) < Tuple( { 0, 1, 0 } ) );
  EXPECT_TRUE(  Tuple( { 0, 0, 0 } ) < Tuple( { 1, 0, 0 } ) );

  EXPECT_TRUE(  Tuple( { 0, 0, 0 } ) < Tuple( { 0, 1, numeric_limits<int>::min() } ) );
  EXPECT_TRUE(  Tuple( { 0, 0, numeric_limits<int>::max() } ) < Tuple( { 0, 1, 0 } ) );
}

TEST( Tuple_test, greater_than ){
  EXPECT_FALSE( Tuple( { 0, 0, 0 } ) > Tuple( { 0, 0, 0 } ) );
  EXPECT_FALSE( Tuple( { 0, 0    } ) > Tuple( { 0, 0, 0 } ) );
  EXPECT_TRUE(  Tuple( { 0, 0, 0 } ) > Tuple( { 0, 0    } ) );

  EXPECT_FALSE(  Tuple( { 0, 0, 0 } ) > Tuple( { 0, 0, 1 } ) );
  EXPECT_FALSE(  Tuple( { 0, 0, 0 } ) > Tuple( { 0, 1, 0 } ) );
  EXPECT_FALSE(  Tuple( { 0, 0, 0 } ) > Tuple( { 1, 0, 0 } ) );

  EXPECT_FALSE(  Tuple( { 0, 0, 0 } ) > Tuple( { 0, 1, numeric_limits<int>::min() } ) );
  EXPECT_FALSE(  Tuple( { 0, 0, numeric_limits<int>::max() } ) > Tuple( { 0, 1, 0 } ) );
}

TEST( Tuple_test, equal_to ){
  EXPECT_TRUE(  Tuple( { 0, 0, 0 } ) == Tuple( { 0, 0, 0 } ) );
  EXPECT_FALSE( Tuple( { 0, 0    } ) == Tuple( { 0, 0, 0 } ) );
  EXPECT_FALSE( Tuple( { 0, 0, 0 } ) == Tuple( { 0, 0    } ) );

  EXPECT_TRUE(  Tuple( { 1, 2, 3 } ) == Tuple( { 1, 2, 3 } ) );
  EXPECT_FALSE( Tuple( { 1, 2, 3 } ) == Tuple( { 1, 0, 3 } ) );
}

TEST( Tuple_test, not_equal_to ){
  EXPECT_FALSE( Tuple( { 0, 0, 0 } ) != Tuple( { 0, 0, 0 } ) );
  EXPECT_TRUE(  Tuple( { 0, 0    } ) != Tuple( { 0, 0, 0 } ) );
  EXPECT_TRUE(  Tuple( { 0, 0, 0 } ) != Tuple( { 0, 0    } ) );

  EXPECT_FALSE( Tuple( { 1, 2, 3 } ) != Tuple( { 1, 2, 3 } ) );
  EXPECT_TRUE(  Tuple( { 1, 2, 3 } ) != Tuple( { 1, 0, 3 } ) );
}


TEST( Tuple_test, add ){
  Tuple a(  { 0, 0, 0 } );
  Tuple b(  { 1, 2, 3 } );
  Tuple b2( { 2, 4, 6 } );
  // Test no throw
  ASSERT_NO_THROW({
    a + a;
    a + b;
    b + b;
  });

  EXPECT_EQ( a + a, a  );
  EXPECT_EQ( a + b, b  );
  EXPECT_EQ( b + b, b2 );

  Tuple c( {0} );
  ASSERT_ANY_THROW( a + c );
}

TEST( Tuple_test, negate ){
  Tuple a(  { 0, 0, 0 } );
  Tuple b(  { 1, 2, 3 } );
  Tuple neg_b(  { -1, -2, -3 } );
  // Test no throw
  ASSERT_NO_THROW({
    -a;
    -b;
    -neg_b;
  });

  EXPECT_EQ( -a, a      );
  EXPECT_EQ( -b, neg_b  );
  EXPECT_EQ( -neg_b, b  );
}

TEST( Tuple_test, subtract ){
  Tuple a(  { 0, 0, 0 } );
  Tuple b(  { 1, 2, 3 } );
  Tuple neg_b(  { -1, -2, -3 } );
  // Test no throw
  ASSERT_NO_THROW({
    a - a;
    a - b;
    b - a;
    b - b;
  });

  EXPECT_EQ( a - a, a     );
  EXPECT_EQ( a - b, neg_b );
  EXPECT_EQ( b - a, b     );
  EXPECT_EQ( b - b, a     );

  Tuple c( {0} );
  ASSERT_ANY_THROW( a - c );
}

TEST( Tuple_test, str ){
  Tuple a(  { 0, 0, 0 } );
  Tuple b(  { 1, 2, 3 } );
  Tuple neg_b(  { -1, -2, -3 } );

  EXPECT_EQ( a.str(), "(0, 0, 0)" );
  EXPECT_EQ( b.str(), "(1, 2, 3)" );
  EXPECT_EQ( neg_b.str(), "(-1, -2, -3)" );
  EXPECT_EQ( (a - b).str(), neg_b.str() );
}

TEST( TupleCollection_test, construct ){
  set<Tuple> init_set = { Tuple( { 0, 0 } ), Tuple( { 0, 1 } ), Tuple( { 1, 0 } ), Tuple( { 1, 0 } ) };
  ASSERT_NO_THROW({ TupleCollection maythrow( init_set ); });

  TupleCollection collection( init_set );

  EXPECT_EQ( collection.dimensions(), 2 );

  for( Tuple tuple : collection ){
    EXPECT_TRUE( init_set.count( tuple ) == 1 );
  }

}

TEST( TupleCollection_test, min_max_OnDims ){
  set<Tuple> init_set = {  Tuple( {  1, -1 } ),
                           Tuple( { -1,  1 } ),
                           Tuple( { -1,  0 } ),
                           Tuple( {  0, -1 } ),
                           Tuple( {  0,  0 } ),
                           Tuple( {  0,  1 } ),
                           Tuple( {  1,  0 } )
                         };

  TupleCollection collection( init_set );
  EXPECT_EQ( collection.minOnDims(), Tuple( { -1, -1 } ) );
  EXPECT_EQ( collection.maxOnDims(), Tuple( {  1,  1 } ) );
}

TEST( TupleCollection_test, shiftAll ){
  set<Tuple> init_set = {  Tuple( {  1, -1 } ),
                           Tuple( { -1,  1 } ),
                           Tuple( { -1,  0 } ),
                           Tuple( {  0, -1 } ),
                           Tuple( {  0,  0 } ),
                           Tuple( {  0,  1 } ),
                           Tuple( {  1,  0 } )
                         };

  TupleCollection collection( init_set );
  Tuple extent( {-1, 1} );
  collection.shiftAll( extent );

  EXPECT_EQ( collection.minOnDims(), Tuple( { -2, 0 } ) );
  EXPECT_EQ( collection.maxOnDims(), Tuple( {  0, 2 } ) );
}

TEST( TupleCollection_test, str ){
  set<Tuple> init_set = {  Tuple( {  1, -1 } ),
                           Tuple( { -1,  1 } ),
                           Tuple( { -1,  0 } ),
                           Tuple( {  0, -1 } ),
                           Tuple( {  0,  0 } ),
                           Tuple( {  0,  1 } ),
                           Tuple( {  1,  0 } )
                         };

  TupleCollection collection( init_set );
  EXPECT_NE( collection.str(), "{  }" );
}

TEST( TupleCollection_test, empty ){
  TupleCollection collection( 3 );
  EXPECT_EQ( collection.minOnDims(), Tuple::createMagicEmptyTuple() );
  EXPECT_EQ( collection.maxOnDims(), Tuple::createMagicEmptyTuple() );
}

TEST( Dataspace_test, construct ){

  TupleCollection read( {  Tuple( {  0, -1 } ),
                           Tuple( { -1,  0 } ),
                           Tuple( {  0,  0 } ),
                           Tuple( {  0,  1 } ),
                           Tuple( {  1,  0 } )
                         } );
  TupleCollection write( {  Tuple( {  0,  0 } ) } );

  ASSERT_NO_THROW({ Dataspace dataspace_a( "A", read, write ); });

  ASSERT_NO_THROW({
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
                  });

  ASSERT_NO_THROW({
    Dataspace dataspace_a(  "A",
                            TupleCollection( 2 ),
                            TupleCollection( {
                              Tuple( {  0,  1 } ),
                              Tuple( {  -1, 0 } )
                            } )
                        );
  });
}
