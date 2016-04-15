/*! ****************************************************************************
\file FusionTransformation.cpp
\autors Ian J. Bertolacci

\brief
Fuses a series of loops nests.

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/

#include "FusionTransformation.hpp"
#include <iostream>
#include <sstream>
#include <iostream>

using namespace LoopChainIR;

FusionTransformation::FusionTransformation( std::vector<LoopChain::size_type> loops ) :
  fusion_loops(loops)
  { }


FusionTransformation::FusionTransformation( LoopChain::size_type loops[], int num_loops ) :
  fusion_loops(loops, loops+num_loops)
  { }

std::string& FusionTransformation::apply( Schedule& schedule ){
  /*
  HowItWorks: FusionTransformation
  In ISCC a fusion transformation of a pair of loops [l1,i,..,ii,s1],
  [l1,i,..,ii,s1] where l1 and l2 are the loop indices and s1 and s2 are the
  statment index, is a mapping of each loop into a common loop index, and a
  reordering of the statements by the original loop index.

  Example:
    # Loops
    S := {s1[i] : 1<=i<=10} + {s2[i] : 1<=i<=10};
    # Map into loop chain
    # s1 is loop 0, s2 is loop 1
    M1 := { s1[i] -> [0,i,0]; s2[i] -> [1,i,0] };
    # Fusion transformation
    M2 := { [0,i,0] -> [0,i,0]; # Map loop 0 to loop 0, body to statement 0
            [1,i,0] -> [0,i,1]; # Map loop 0 to loop 0, body to statement 1
          };
    codegen ((M1.M2)*S);

  This can be made more general, and must be to pass-through loops not targeted
  by the fusion.
  Our input iterations (in all cases) is [f,i,..,ii,s]
  Again, f (from) is the loop index, and s is the statement index.
  For the targeted loops we map to [t,i,..,ii,f] where t (to) is the common
  loop index.
  Notice that s is discarded in the transformation.
  Also notice that we use f to order the statements in the fusion.
  This means that the order of the statements within the fused loop body always
  matches the order of the statements w.r.t. the fused loops.

  Example:
    # Individual loops
    s1 := [N] -> {s0[idx_0] : 1 <= idx_0 <= N} ;
    s2 := [N] -> {s1[idx_0] : 1 <= idx_0 <= N} ;
    # Union loops
    S := s1 + s2;
    # Map into loop chain
    M1 := {s0[idx_0] -> [0,idx_0,0]; s1[idx_0] -> [1,idx_0,0]; };
    # Fusion Transformation
    M2 := {
          # Fuse target loops
          [f,i,ii] -> [t,i,f] : ( t = 0 ) and (f = 0 or f = 1);
          # Identity trasformation (pass-through)
          [f,i,ii] -> [f,i,ii] : !(f = 0 or f = 1)
          };

    codegen ((M1 . M2 ) * S);
  */

  std::ostringstream input_iteration;
  std::ostringstream output_iteration;
  std::ostringstream source;
  std::ostringstream target;

  // to/from iterators
  input_iteration << "f";
  output_iteration << "t";

  // Create rest of input iteration
  for( RectangularDomain::size_type i = 1; i < schedule.getIteratorsLength(); i += 1 ){
    // Create unique symbol for iterator ("i"+i, eg "i1", "i2", ...)
    input_iteration << ",i" << i;
  }

  // Create rest of output iteration
  // Notice we are creating one less iterator (lacks the statement iterator)
  for( RectangularDomain::size_type i = 1; i < schedule.getIteratorsLength()-1; i += 1 ){
    // Create unique symbol for iterator ("i"+i, eg "i1", "i2", ...)
    output_iteration << ",i" << i;
  }

  // Add statement iterator to output_iteration
  output_iteration << ",f";


  // Create conditional expression for source loops (eg (f=0 or f=1 or ...))
  source << "(";
  for( iterator it = this->begin(); it != this->end(); ++it ){
    if( it != this->begin() ) source << " or " ;
    source << "f = " << SSTR( *it ) ;
  }
  source << ")";

  // Create conditional expression for target loops (eg (t=0))
  target << "( t = " << this->fusion_loops[0] << " )" ;

  // Create our transformation mapping
  std::ostringstream transformation;
  transformation << "{" << "\n"
                 // Fusion transformation for targeted loops
                 << "[" << input_iteration.str() << "] -> [" << output_iteration.str() << "] : " << target.str() << " and " << source.str() << ";\n"
                 // Identity transformation for pass-through
                 << "[" << input_iteration.str() << "] -> [" << input_iteration.str() << "] : !" << source.str() << "\n"
                 << "};";
  return *(new std::string(transformation.str()));
}

FusionTransformation::iterator FusionTransformation::begin(){
  return this->fusion_loops.begin();
}

FusionTransformation::iterator FusionTransformation::end(){
  return this->fusion_loops.end();
}
