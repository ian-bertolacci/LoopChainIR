/*! ****************************************************************************
\file ShiftTransformation.cpp
\autors Ian J. Bertolacci

\brief
Shift a single loop nest by some extent

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/

#include "ShiftTransformation.hpp"
#include <iostream>
#include <sstream>
#include "util.hpp"

/*!
\brief
Shift all loops specified in nest by the extents.
\param[in] loop The id of the loop-nest within the chain to be shifted
\param[in] extents The extent by which the loops are shifted by.
           Note that extents.size() must be equal to
           chain.getNest( loop ).getDomain().dimensions(), where chain is the
           LoopChain on which the transformations are being applied to.
*/
ShiftTransformation::ShiftTransformation( LoopChain::size_type loop, std::vector<std::string> extents )
  : loop_id(loop), extents(extents)
  { }


/*!
\brief
Shift all loops specified in nest by the extents.
\param[in] loop The id of the loop-nest within the chain to be shifted
\param[in] extents The extent by which the loops are shifted by.
           Note that extents.size() must be equal to
           chain.getNest( loop ).getDomain().dimensions(), where chain is the
           LoopChain on which the transformations are being applied to.
\param[in] symbols Symbols found within any of the extent expressions.
*/
ShiftTransformation::ShiftTransformation( LoopChain::size_type loop, std::vector<std::string> extents, std::vector<std::string> symbols )
  : loop_id(loop), extents(extents), symbols(symbols)
  { }


/*!
\brief
Shift all loops specified in nest by the extent.
\param[in] loop The id of the loop-nest within the chain to be shifted
\param[in] extent The extent by which the loops are shifted by
*/
ShiftTransformation::ShiftTransformation( LoopChain::size_type loop, std::string extent )
  : loop_id(loop), extents(1, extent)
  { }

/*!
\brief
Shift all loops specified in nest by the extent.
\param[in] loop The id of the loop-nest within the chain to be shifted
\param[in] extent The extent by which the loops are shifted by
\param[in] symbols Symbols found within the extent expression.
*/
ShiftTransformation::ShiftTransformation( LoopChain::size_type loop, std::string extent, std::vector<std::string> symbols )
  : loop_id(loop), extents(1, extent), symbols(symbols)
  { }


/*!
\returns The loop id within the associated loop chain.
*/
LoopChain::size_type ShiftTransformation::getLoopId(){
  return this->loop_id;
}

/*!
\returns A vector of the extents;
*/
std::vector<std::string> ShiftTransformation::getExtents(){
  return std::vector<std::string>(this->extents);
}

/*!
\returns A vector of the symbols within the extents;
*/
std::vector<std::string> ShiftTransformation::getSymbols(){
  return std::vector<std::string>(this->symbols);
}


std::string& ShiftTransformation::apply( Schedule& schedule ){
  LoopChain chain = schedule.getChain();

  assertWithException( this->extents.size() == chain.getNest(this->loop_id).getDomain().dimensions(),
                       SSTR( "Dimensionality of extent of ShiftTransformation on loop "
                           << this->loop_id << " is greater ("
                           << this->extents.size()
                           << ") than the dimensionality of the domain ("
                           << chain.getNest(this->loop_id).getDomain().dimensions() << ")"  )
                     )

  /*
  HowItWorks: ShiftTransformation
  In ISCC, a shift transformation on a loop nest [l,i,ii,...,s]
  is the mapping from an input iteration [t,i,...,ii] to an output iteration
  where the extent of the shift for each dimension is added to that dimension's
  iterator, [t,i+(e_i),...ii+(e_ii),s], where t is the loop being shifted.
  We can keep all other iterators general, as their specific values are
  1) unknown, and 2) unimportant

  For example, a shift of 5 on a 1N_1D chain:
  # Loop
  S := [N]->{ [0,i,0] : 1 <= i <= N};
  M := { [0,i,ii] -> [0,i+5,ii]};

  Note that the last iteartor, ii, is the sataement position and has no shift
  applied to it.

  For arbitrary nests we need a pass-through mapping as well, to preserve
  iterations not touched by the shift.

  For example, a shift of k on some loop chain, where the target loop
  (loop 1) is 1D:
  M := [k]->{
              [t,i,ii] -> [t,i+(K),ii] : t = 1; # Transformation mapping
              [t,i,ii] -> [t,i,ii] : t != 1 ; # Identity (pass-through mapping)
            };

  Runable ISCC Example:
    # 2N_1D loop nest
    S := [N]->{s1[i] : 1 <= i <= N} + [M]->{s2[j] : 1 <= j <= M };
    # Map Statements into loops
    M1 := {s1[i] -> [0,i,0] ; s2[i]->[1,i,0]};
    # Shift loop 0 by 10
    M2 := {
    [t,i,ii]->[t,i+10,ii] : (t=0);
    [t,i,ii]->[t,i,ii] : (t!=0)
    };
    codegen ((M1.M2)*S);

  Output:
    {
    for (int c1 = 11; c1 <= N + 10; c1 += 1)
      s1(c1 - 10);
    for (int c1 = 1; c1 <= M; c1 += 1)
      s2(c1);
    }

  */

  std::ostringstream input_iteration;
  std::ostringstream output_iteration;

  input_iteration << "t";
  output_iteration << "t";

  // goes to getIteratorsLength()-1 because we alreay put t on.
  // But, still need to index into extent (extent[i])
  for( RectangularDomain::size_type i = 0; i < schedule.getIteratorsLength()-1; i += 1 ){
    input_iteration << ",i" << i;
    output_iteration << ",i" << i;

    // Add this dimension's extent to the iterator
    if( i < this->extents.size() ){
      output_iteration << "+(" << this->extents[i] << ")";
    }
  }

  std::ostringstream transformation;

  // Prepend symbols to mapping expression (e.g. "[N,M,K]->")
  if( !this->symbols.empty() ){
    transformation << "[";
    for( std::vector<std::string>::iterator it = this->symbols.begin(); it != this->symbols.end(); it++ ){
      if( it != this->symbols.begin() ){
        transformation << ",";
      }
      transformation << *it;
    }
    transformation << "]->";
  }

  // Create our mapping expression from the input iteration
  // to the output iteration.
  transformation << "{" << "\n"
                 // Shift transformation for targeted loops
                 << "[" << input_iteration.str() << "]->[" << output_iteration.str() << "] : " << "(t = " << this->loop_id << ")" << ";\n"
                 // Identity transformation for pass-through
                 << "[" << input_iteration.str() << "]->[" << input_iteration.str() << "] : " << "(t != " << this->loop_id << ")" << ";\n"
                 << "}";

  return *(new std::string(transformation.str()));
}
