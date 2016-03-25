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

  std::ostringstream input_iteration;
  std::ostringstream output_iteration;

  input_iteration << "t"; // this->loop_id;
  output_iteration << "t"; // this->loop_id;

  // goes to getIteratorsLength()-1 because we alreay put loop_id in.
  // But, still need to index into extent (extent[i])
  // Could shift forward and do extent[i-1]
  for( RectangularDomain::size_type i = 0; i < schedule.getIteratorsLength()-1; i += 1 ){
    input_iteration << ",";
    output_iteration << ",";

    for(RectangularDomain::size_type m = 0; m <= i; m += 1 ){
      input_iteration << "i";
      output_iteration << "i";
    }

    if( i < this->extents.size() ){
      output_iteration << "+(" << this->extents[i] << ")";
    }
  }

  std::ostringstream mapping;

  if( !this->symbols.empty() ){
    mapping << "[";
    for( std::vector<std::string>::iterator it = this->symbols.begin(); it != this->symbols.end(); it++ ){
      if( it != this->symbols.begin() ){
        mapping << ",";
      }
      mapping << *it;
    }
    mapping << "]->";
  }

  mapping << "{" << "\n"
          << "[" << input_iteration.str() << "]->[" << output_iteration.str() << "] :" << "(t = " << this->loop_id << ")" << ";\n"
          << "[" << input_iteration.str() << "]->[" << input_iteration.str() << "] :" << "(t != " << this->loop_id << ")" << ";\n"
          << "}";

  return *(new std::string(mapping.str()));
}
