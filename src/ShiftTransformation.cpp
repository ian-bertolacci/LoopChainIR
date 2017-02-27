/*! ****************************************************************************
\file ShiftTransformation.cpp
\authors Ian J. Bertolacci

\brief
Shift a single loop nest by some extent

\copyright
Copyright 2015-2016 Colorado State University
Copyright 2017 Universiy of Arizona
*******************************************************************************/

#include "ShiftTransformation.hpp"
#include <iostream>
#include <sstream>
#include "util.hpp"

using namespace LoopChainIR;
using namespace std;

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


std::vector<std::string> ShiftTransformation::apply( Schedule& schedule ){
  return this->apply( schedule, *(std::next(schedule.getSubspaceManager().get_iterator_to_loops())) );
}

std::vector<std::string> ShiftTransformation::apply( Schedule& schedule, Subspace* subspace ){
  vector<std::string> transformations;

  SubspaceManager& manager = schedule.getSubspaceManager();
  Subspace* loops = manager.get_loops();

  // Alias the shifting subspace
  subspace->set_aliased();

  // Assert that there are the same number of extents as there are variable iterators in the shifted subspace.
  assertWithException( this->extents.size() == subspace->size() ,
                       SSTR( "Dimensionality of extent of ShiftTransformation on loop "
                           << this->loop_id << " is not equal ("
                           << this->extents.size()
                           << ") to the dimensionality of the Subspace ("
                           << subspace->size() << ")"  )
                     );

  // String stream transformation will be created in.
  ostringstream transformation;

  // Put in sumbolic constraints (eg "[N,M]->")
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

  // Create funtion header,
  transformation  << "{ \n\t["
                  << manager.get_input_iterators() << "] -> ["
                  << manager.get_output_iterators() << "] : \n\t\t"
  // Create condition to only map target loop
                  << loops->get( loops->const_index, false )
                  << " = " << this->loop_id << "\n\t\t";

  // Create conditions to shift subspace
  std::vector<std::string> extents = this->getExtents();
  for( Subspace::size_type i = 0; i < subspace->complete_size(); ++i ){
    // get aliased symbol
    transformation << " and " << (*subspace)[i] << " = " << subspace->get(i, false);
    // add extent
    if( i < this->extents.size() ){
      transformation  << "+(" << this->extents[i] << ")";
    }
  }

  // End shift component of mapping
  transformation << ";\n\t";

  // Start pass-through component of mapping
  // Unalias subspace
  subspace->unset_aliased();

  // Create map header
  transformation  << "["
                  << manager.get_input_iterators() << "] -> ["
                  << manager.get_output_iterators() << "] : \n\t\t"
  // Create condition to map non-target loops
                  << loops->get( loops->const_index, false )
                  << " != " << this->loop_id;
  // End mapping
  transformation << "; \n};";

  // Add transformation to our list
  transformations.push_back( transformation.str() );
  // Return list of created transformations.
  return transformations;
}
