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
#include <algorithm>
#include "util.hpp"

using namespace LoopChainIR;
using namespace std;

ShiftTransformation::ShiftTransformation( LoopChain::size_type loop, Tuple extent )
: loop_id( loop ), extent( extent )
{ }

ShiftTransformation::ShiftTransformation( LoopChain::size_type loop, std::string extent )
: ShiftTransformation( loop, Tuple( { extent } ) )
{ }

ShiftTransformation::ShiftTransformation( LoopChain::size_type loop, std::vector<std::string> extents )
: ShiftTransformation( loop, Tuple( extents ) )
{ }


LoopChain::size_type ShiftTransformation::getLoopId(){
  return this->loop_id;
}

Tuple ShiftTransformation::getExtents(){
  return this->extent;
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
  assertWithException( this->extent.dimensions() == subspace->size() ,
                       SSTR( "Dimensionality of extent of ShiftTransformation on loop "
                           << this->loop_id << " is not equal ("
                           << this->extent.dimensions()
                           << ") to the dimensionality of the Subspace ("
                           << subspace->size() << ")"  )
                     );

  // String stream transformation will be created in.
  ostringstream transformation;

  /*
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
  */

  // Create funtion header,
  transformation  << "{ \n\t["
                  << manager.get_input_iterators() << "] -> ["
                  << manager.get_output_iterators() << "] : \n\t\t"
  // Create condition to only map target loop
                  << loops->get( loops->const_index, false )
                  << " = " << this->loop_id << "\n\t\t";

  // Create conditions to shift subspace
  for( Subspace::size_type i = 0; i < subspace->complete_size(); ++i ){
    // get aliased symbol
    transformation << " and " << (*subspace)[i] << " = " << subspace->get(i, false);
    // add extent
    if( i < this->extent.dimensions() ){
      transformation  << "+(" << this->extent[i] << ")";
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

  // Modify shifts
  schedule.getChain().getNest( this->loop_id ).shiftDataspaces( this->extent );

  // Return list of created transformations.
  return transformations;
}
