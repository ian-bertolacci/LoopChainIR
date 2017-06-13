#include "gtest/gtest.h"
#include <LoopChainIR/WavefrontTransformation.hpp>
#include <LoopChainIR/FusionTransformation.hpp>
#include <iostream>
#include <utility>

using namespace std;
using namespace LoopChainIR;

TEST(WavefrontTransformationTest, test_1) {
  LoopChain chain;

  chain.append(
    LoopNest(
      RectangularDomain( { make_pair( "1", "N"), make_pair( "1", "M") }, {"N","M"} ),
      {
        Dataspace(  "A",
                    TupleCollection( {
                      Tuple( { -1,  0 } ),
                      Tuple( {  0, -1 } ),
                      Tuple( {  0,  0 } ),
                      Tuple( {  0,  1 } ),
                      Tuple( {  1,  0 } )
                    } ),
                    TupleCollection( 2 )
                ),
        Dataspace(  "B",
                    TupleCollection( {
                      Tuple( { 0, 0 } )
                    } ),
                    TupleCollection( 2 )
                )
      }
    )
  );

  chain.append(
    LoopNest(
      RectangularDomain( { make_pair( "1", "N"), make_pair( "1", "M") }, {"N","M"} ),
      {
        Dataspace(  "A",
                    TupleCollection( {
                      Tuple( { 0, 0 } )
                    } ),
                    TupleCollection( 2 )
                ),
        Dataspace(  "B",
                    TupleCollection( {
                      Tuple( { -1,  0 } ),
                      Tuple( {  0, -1 } ),
                      Tuple( {  0,  0 } ),
                      Tuple( {  0,  1 } ),
                      Tuple( {  1,  0 } )
                    } ),
                    TupleCollection( 2 )
                )
      }
    )
  );

  Schedule sched( chain );

  // Create an ordered list of Transformations
  vector<Transformation*> transformations = {
    new FusionTransformation( (std::vector<LoopChain::size_type>){0,1} ),
    new WavefrontTransformation()
  };

  ASSERT_NO_THROW({ sched.apply( transformations ); });
  ASSERT_NO_THROW({ sched.codegen(); });
  ASSERT_NE( sched.codegen(), string("{\n}\n") );

}
