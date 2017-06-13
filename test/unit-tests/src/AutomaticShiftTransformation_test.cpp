#include "gtest/gtest.h"
#include <LoopChainIR/AutomaticShiftTransformation.hpp>
#include <iostream>
#include <utility>
#include <vector>
#include <set>

using namespace std;
using namespace LoopChainIR;

TEST( AutomaticShiftTransformation_test, noshift){
  LoopChain chain;

  string lower[1] = {"0"};
  string upper[1] = {"10"};

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
        )
      }
    )
  );

  std::map<LoopChain::size_type, Tuple> shift_tuples = AutomaticShiftTransformation::computeShiftTuplesForFusion( 1, chain, true );
  ASSERT_EQ( shift_tuples.size(), 0 );
}

TEST( AutomaticShiftTransformation_test, negative){
  LoopChain chain;

  string lower[1] = {"0"};
  string upper[1] = {"10"};

  chain.append(
    LoopNest(
      RectangularDomain( lower, upper, 1 ),
      {
        Dataspace(
          "A",
          // Reads
          TupleCollection( 1 ),
          // writes
          TupleCollection({
            Tuple({ 0 }),
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
          }),
          // writes
          TupleCollection( 1 )
        )
      }
    )
  );

  std::map<LoopChain::size_type, Tuple> actual = {
                                                    make_pair<LoopChain::size_type, Tuple>( 0, Tuple({1}) ),
                                                    make_pair<LoopChain::size_type, Tuple>( 1, Tuple({0}) ),
                                                  };
  std::map<LoopChain::size_type, Tuple> shift_tuples = AutomaticShiftTransformation::computeShiftTuplesForFusion( 1, chain, true );

  ASSERT_EQ( actual.size(), shift_tuples.size() );
  for(  std::map<LoopChain::size_type, Tuple>::iterator a_it = actual.begin(), s_it = shift_tuples.begin();
        a_it != actual.end() && s_it != shift_tuples.end();
        ++a_it, ++s_it ){
    EXPECT_EQ( *a_it, *s_it );
  }
}

TEST( AutomaticShiftTransformation_test, jacobi1d_unrolled){
  LoopChain chain;

  string lower[1] = { "1" };
  string upper[1] = { "10" };

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

  std::map<LoopChain::size_type, Tuple> actual = {
                                make_pair<LoopChain::size_type, Tuple>( 0, Tuple({0}) ),
                                make_pair<LoopChain::size_type, Tuple>( 1, Tuple({1}) ),
                              };

  std::map<LoopChain::size_type, Tuple> shift_tuples = AutomaticShiftTransformation::computeShiftTuplesForFusion( 1, chain, true );

  ASSERT_EQ( actual.size(), shift_tuples.size() );
  for(  std::map<LoopChain::size_type, Tuple>::iterator a_it = actual.begin(), s_it = shift_tuples.begin();
        a_it != actual.end() && s_it != shift_tuples.end();
        ++a_it, ++s_it ){
    EXPECT_EQ( *a_it, *s_it );
  }
}

TEST( AutomaticShiftTransformation_test, jacobi2d_unrolled){
  LoopChain chain;
  string lower[2] = { "1","1" };
  string upper[2] = { "N","N" };

  chain.append(
    LoopNest(
      RectangularDomain( lower, upper, 1 ),
      {
        Dataspace(
          "B",
          // Reads
          TupleCollection({
            Tuple({ -1,  0 }),
            Tuple({  0, -1 }),
            Tuple({  1,  0 }),
            Tuple({  0,  1 }),
            Tuple({  0,  0 }),
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
            Tuple({  1,  0 }),
            Tuple({  0,  1 }),
            Tuple({  0,  0 }),
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
            Tuple({ 0,  0 }),
          })
        )
      }
    )
  );

  std::map<LoopChain::size_type, Tuple> actual = {
    make_pair<LoopChain::size_type, Tuple>( 0, Tuple({0, 0}) ),
    make_pair<LoopChain::size_type, Tuple>( 1, Tuple({1, 1}) ),
  };

  std::map<LoopChain::size_type, Tuple> shift_tuples = AutomaticShiftTransformation::computeShiftTuplesForFusion( 2, chain, true );

  ASSERT_EQ( actual.size(), shift_tuples.size() );
  for(  std::map<LoopChain::size_type, Tuple>::iterator a_it = actual.begin(), s_it = shift_tuples.begin();
        a_it != actual.end() && s_it != shift_tuples.end();
        ++a_it, ++s_it ){
    EXPECT_EQ( *a_it, *s_it );
  }
}

TEST( AutomaticShiftTransformation_test, jacobi2d_unrolled_2forward){
  LoopChain chain;
  string lower[2] = { "1","1" };
  string upper[2] = { "N","N" };

  chain.append(
    LoopNest(
      RectangularDomain( lower, upper, 1 ),
      {
        Dataspace(
          "B",
          // Reads
          TupleCollection({
            Tuple({ -1,  0 }),
            Tuple({  0, -1 }),
            Tuple({  1,  0 }),
            Tuple({  0,  1 }),
            Tuple({  0,  0 }),
            Tuple({  0,  2 }),
            Tuple({  2,  0 }),
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
            Tuple({  1,  0 }),
            Tuple({  0,  1 }),
            Tuple({  0,  0 }),
            Tuple({  0,  2 }),
            Tuple({  2,  0 }),
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
            Tuple({ 0,  0 }),
          })
        )
      }
    )
  );

  std::map<LoopChain::size_type, Tuple> actual = {
    make_pair<LoopChain::size_type, Tuple>( 0, Tuple({0, 0}) ),
    make_pair<LoopChain::size_type, Tuple>( 1, Tuple({2, 2}) ),
  };

  std::map<LoopChain::size_type, Tuple> shift_tuples = AutomaticShiftTransformation::computeShiftTuplesForFusion( 2, chain, true );

  ASSERT_EQ( actual.size(), shift_tuples.size() );
  for(  std::map<LoopChain::size_type, Tuple>::iterator a_it = actual.begin(), s_it = shift_tuples.begin();
        a_it != actual.end() && s_it != shift_tuples.end();
        ++a_it, ++s_it ){
    EXPECT_EQ( *a_it, *s_it );
  }
}

TEST( AutomaticShiftTransformation_test, MFD_2D_TripleCache ){
  LoopChain chain;

  std::set<std::string> symbols = { "numcells" };

  {
    string lower[2] = { "0", "0" };
    string upper[2] = { "numCell-1", "numCell" };

    // L0
    chain.append(
      LoopNest(
        RectangularDomain( lower, upper, 2, symbols ),
        {
          Dataspace(  "CACHEX",
                      TupleCollection( 2 ),
                      TupleCollection({ Tuple({ 0,0 }) })
                    ),
          Dataspace(  "OLD",
                      TupleCollection({ Tuple({ 0,-2 }), Tuple({ 0,-1 }), Tuple({ 0,0 }), Tuple({ 0,1 }) }),
                      TupleCollection( 2 )
                    )
        }
      )
    );
  }

  // L1
  {
    string lower[2] = { "0", "0" };
    string upper[2] = { "numCell-1", "numCell-1" };

    chain.append(
      LoopNest(
        RectangularDomain( lower, upper, 2, symbols ),
        {
          Dataspace(  "CACHEX",
                      TupleCollection({ Tuple({ 0,0 }) }),
                      TupleCollection({ Tuple({ 0,0 }) })
                    )
        }
      )
    );
  }


  // L2
  {
    string lower[2] = { "0", "0" };
    string upper[2] = { "numCell", "numCell-1" };

    chain.append(
      LoopNest(
        RectangularDomain( lower, upper, 2, symbols ),
        {
          Dataspace(  "NEW",
                      TupleCollection( 2 ),
                      TupleCollection({ Tuple({ 0,0 }) })
                    ),
          Dataspace(  "CACHEX",
                      TupleCollection({ Tuple({ 0,1 }), Tuple({ 0,0 }) }),
                      TupleCollection( 2 )
                    )
        }
      )
    );
  }


  // L3
  {
    string lower[2] = { "0", "0" };
    string upper[2] = { "numCell", "numCell-1" };

    chain.append(
      LoopNest(
        RectangularDomain( lower, upper, 2, symbols ),
        {
          Dataspace( "CACHEY",
                      TupleCollection( 2 ),
                      TupleCollection({ Tuple({ 0,0 }) })
                    ),
          Dataspace(  "OLD",
                      TupleCollection({ Tuple({ -2,0 }),Tuple({ -1,0 }),Tuple({ 0,0 }),Tuple({ 1,0 }) }),
                      TupleCollection( 2 )
                    )
        }
      )
    );
  }


  // L4
  {
    string lower[2] = { "0", "0" };
    string upper[2] = { "numCell-1", "numCell-1" };

    chain.append(
      LoopNest(
        RectangularDomain( lower, upper, 2, symbols ),
        {
          Dataspace(  "CACHEY",
                      TupleCollection({ Tuple({ 0,0 }) }),
                      TupleCollection({ Tuple({ 0,0 }) })
                    )
        }
      )
    );
  }


  // L5
  {
    string lower[2] = { "0", "0" };
    string upper[2] = { "numCell-1", "numCell-1" };

    chain.append(
      LoopNest(
        RectangularDomain( lower, upper, 2, symbols ),
        {
          Dataspace(  "NEW",
                      TupleCollection( 2 ),
                      TupleCollection({ Tuple({ 0,0 }) })
                    ),
          Dataspace(  "CACHEY",
                      TupleCollection({ Tuple({ 1,0 }),Tuple({ 0,0 }) }),
                      TupleCollection( 2 )
                    )
        }
      )
    );
  }

  std::map<LoopChain::size_type, Tuple> actual = {
                                make_pair<LoopChain::size_type, Tuple>( 0, Tuple({1, 0}) ),
                                make_pair<LoopChain::size_type, Tuple>( 1, Tuple({1, 0}) ),
                                make_pair<LoopChain::size_type, Tuple>( 2, Tuple({1, 1}) ),
                                make_pair<LoopChain::size_type, Tuple>( 3, Tuple({0, 1}) ),
                                make_pair<LoopChain::size_type, Tuple>( 4, Tuple({0, 1}) ),
                                make_pair<LoopChain::size_type, Tuple>( 5, Tuple({1, 1}) ),
                              };

  std::map<LoopChain::size_type, Tuple> shift_tuples = AutomaticShiftTransformation::computeShiftTuplesForFusion( 2, chain, true );

  ASSERT_EQ( actual.size(), shift_tuples.size() );
  for(  std::map<LoopChain::size_type, Tuple>::iterator a_it = actual.begin(), s_it = shift_tuples.begin();
        a_it != actual.end() && s_it != shift_tuples.end();
        ++a_it, ++s_it ){
    EXPECT_EQ( *a_it, *s_it );
  }
}

TEST( AutomaticShiftTransformation_test, MFD_TripleCache ){
  LoopChain chain;

  std::set<std::string> symbols = { "numcells" };

  {
    string lower[3] = { "0", "0", "0" };
    string upper[3] = { "numCell-1","numCell-1","numCell" };

    // L0
    chain.append(
      LoopNest(
        RectangularDomain( lower, upper, 3, symbols ),
        {
          Dataspace(  "CACHEX",
                      TupleCollection( 3 ),
                      TupleCollection({ Tuple({ 0,0,0 }) })
                    ),
          Dataspace(  "OLD",
                      TupleCollection({ Tuple({ 0,0,-2 }), Tuple({ 0,0,-1 }), Tuple({ 0,0,0 }), Tuple({ 0,0,1 }) }),
                      TupleCollection( 3 )
                    )
        }
      )
    );
  }

  // L1
  {
    string lower[3] = { "0", "0", "0" };
    string upper[3] = { "numCell-1","numCell-1","numCell-1" };

    chain.append(
      LoopNest(
        RectangularDomain( lower, upper, 3, symbols ),
        {
          Dataspace(  "CACHEX",
                      TupleCollection({ Tuple({ 0,0,0 }) }),
                      TupleCollection({ Tuple({ 0,0,0 }) })
                    )
        }
      )
    );
  }


  // L2
  {
    string lower[3] = { "0", "0", "0" };
    string upper[3] = { "numCell-1","numCell","numCell-1" };

    chain.append(
      LoopNest(
        RectangularDomain( lower, upper, 3, symbols ),
        {
          Dataspace(  "NEW",
                      TupleCollection( 3 ),
                      TupleCollection({ Tuple({ 0,0,0 }) })
                    ),
          Dataspace(  "CACHEX",
                      TupleCollection({ Tuple({ 0,0,1 }), Tuple({ 0,0,0 }) }),
                      TupleCollection( 3 )
                    )
        }
      )
    );
  }


  // L3
  {
    string lower[3] = { "0", "0", "0" };
    string upper[3] = { "numCell-1","numCell","numCell-1" };

    chain.append(
      LoopNest(
        RectangularDomain( lower, upper, 3, symbols ),
        {
          Dataspace( "CACHEY",
                      TupleCollection( 3 ),
                      TupleCollection({ Tuple({ 0,0,0 }) })
                    ),
          Dataspace(  "OLD",
                      TupleCollection({ Tuple({ 0,-2,0 }),Tuple({ 0,-1,0 }),Tuple({ 0,0,0 }),Tuple({ 0,1,0 }) }),
                      TupleCollection( 3 )
                    )
        }
      )
    );
  }


  // L4
  {
    string lower[3] = { "0", "0", "0" };
    string upper[3] = { "numCell-1","numCell-1","numCell-1" };

    chain.append(
      LoopNest(
        RectangularDomain( lower, upper, 3, symbols ),
        {
          Dataspace(  "CACHEY",
                      TupleCollection({ Tuple({ 0,0,0 }) }),
                      TupleCollection({ Tuple({ 0,0,0 }) })
                    )
        }
      )
    );
  }


  // L5
  {
    string lower[3] = { "0", "0", "0" };
    string upper[3] = { "numCell","numCell-1","numCell-1" };

    chain.append(
      LoopNest(
        RectangularDomain( lower, upper, 3, symbols ),
        {
          Dataspace(  "NEW",
                      TupleCollection( 3 ),
                      TupleCollection({ Tuple({ 0,0,0 }) })
                    ),
          Dataspace(  "CACHEY",
                      TupleCollection({ Tuple({ 0,1,0 }),Tuple({ 0,0,0 }) }),
                      TupleCollection( 3 )
                    )
        }
      )
    );
  }


  // L6
  {
    string lower[3] = { "0", "0", "0" };
    string upper[3] = { "numCell","numCell-1","numCell-1" };

    chain.append(
      LoopNest(
        RectangularDomain( lower, upper, 3, symbols ),
        {
          Dataspace(  "CACHEZ",
                      TupleCollection( 3 ),
                      TupleCollection({ Tuple({ 0,0,0 }) })
                    ),
          Dataspace(  "OLD",
                      TupleCollection({ Tuple({ -2,0,0 }),Tuple({ -1,0,0 }),Tuple({ 0,0,0 }),Tuple({ 1,0,0 }) }),
                      TupleCollection( 3 )
                    )
        }
      )
    );
  }


  // L7
  {
    string lower[3] = { "0", "0", "0" };
    string upper[3] = { "numCell","numCell-1","numCell-1" };

    chain.append(
      LoopNest(
        RectangularDomain( lower, upper, 3, symbols ),
        {
          Dataspace(  "CACHEZ",
                      TupleCollection({ Tuple({ 0,0,0 }) }),
                      TupleCollection({ Tuple({ 0,0,0 }) })
                    )
        }
      )
    );
  }


  // L8
  {
    string lower[3] = { "0", "0", "0" };
    string upper[3] = { "numCell-1","numCell-1","numCell-1" };

    chain.append(
      LoopNest(
        RectangularDomain( lower, upper, 3, symbols ),
        {
          Dataspace(  "NEW",
                      TupleCollection( 3 ),
                      TupleCollection({ Tuple({ 0,0,0 }) })
                    ),
          Dataspace(  "CACHEZ",
                      TupleCollection({ Tuple({ 1,0,0 }),Tuple({ 0,0,0 }) }),
                      TupleCollection( 3 )
                    )
        }
      )
    );
  }

  std::map<LoopChain::size_type, Tuple> actual = {
                                make_pair<LoopChain::size_type, Tuple>( 0, Tuple({1, 1, 0}) ),
                                make_pair<LoopChain::size_type, Tuple>( 1, Tuple({1, 1, 0}) ),
                                make_pair<LoopChain::size_type, Tuple>( 2, Tuple({1, 1, 1}) ),
                                make_pair<LoopChain::size_type, Tuple>( 3, Tuple({1, 0, 1}) ),
                                make_pair<LoopChain::size_type, Tuple>( 4, Tuple({1, 0, 1}) ),
                                make_pair<LoopChain::size_type, Tuple>( 5, Tuple({1, 1, 1}) ),
                                make_pair<LoopChain::size_type, Tuple>( 6, Tuple({0, 1, 1}) ),
                                make_pair<LoopChain::size_type, Tuple>( 7, Tuple({0, 1, 1}) ),
                                make_pair<LoopChain::size_type, Tuple>( 8, Tuple({1, 1, 1}) ),
                              };

  std::map<LoopChain::size_type, Tuple> shift_tuples = AutomaticShiftTransformation::computeShiftTuplesForFusion( 3, chain, true );

  ASSERT_EQ( actual.size(), shift_tuples.size() );
  for(  std::map<LoopChain::size_type, Tuple>::iterator a_it = actual.begin(), s_it = shift_tuples.begin();
        a_it != actual.end() && s_it != shift_tuples.end();
        ++a_it, ++s_it ){
    EXPECT_EQ( *a_it, *s_it );
  }
}
