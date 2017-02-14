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

using namespace LoopChainIR;

FusionTransformation::FusionTransformation( std::vector<LoopChain::size_type> loops ) :
  fusion_loops(loops)
  { }


FusionTransformation::FusionTransformation( LoopChain::size_type loops[], int num_loops ) :
  fusion_loops(loops, loops+num_loops)
  { }

std::vector<std::string> FusionTransformation::apply( Schedule& schedule ){
  return this->apply( schedule, schedule.getSubspaceManager().get_loops() );
}

std::vector<std::string> FusionTransformation::apply( Schedule& schedule, Subspace* subspace ){
  std::vector<std::string> transformations;
  SubspaceManager& manager = schedule.getSubspaceManager();

  std::ostringstream transformation;

  assertWithException( std::next(manager.get_iterator_to_subspace( subspace )) != manager.end(), "Given subspace is the last subspace. Cannot form fusion." );
  Subspace* next_subspace = *(std::next(manager.get_iterator_to_subspace( subspace )));

  subspace->set_aliased();
  next_subspace->set_aliased();

  bool is_first;

  transformation << "{ \n\t["
                 << manager.get_input_iterators() << "] -> ["
                 << manager.get_output_iterators() << "] : \n\t\t"
                 << subspace->get(subspace->const_index, true) << " = 0 and "
                 << next_subspace->get(next_subspace->const_index, true)
                 << " = " << subspace->get(subspace->const_index, false);

  // Only map to listed loops
  transformation << " and (";
  is_first = true;
  for( LoopChain::size_type loop_idx : *this ){
    transformation << (!is_first?" or ":"") << subspace->get(subspace->const_index, false)
                   << " = " << loop_idx;
    is_first = false;
  }
  transformation << ")\n\t\t";

  // identity map subspace variable_iterators;
  for( Subspace::size_type i = 0; i < subspace->size(); ++i ){
    transformation << " and " << subspace->get(i, true) << " = " << subspace->get(i, false);
  }
  transformation << "\n\t\t";
  // identity map next_subspace variable_iterators;
  for( Subspace::size_type i = 0; i < next_subspace->size(); ++i ){
    transformation << " and " << next_subspace->get(i, true) << " = " << next_subspace->get(i, false);
  }

  transformation << ";\n\t";

  // Identity map to non-fused loops
  transformation << "["  << manager.get_input_iterators() << "] -> ["
                 << manager.get_output_iterators() << "] : \n\t\t";
  // Only map to non-listed loops
  is_first = true;
  for( LoopChain::size_type loop_idx : *this ){
    transformation << (!is_first?" and ":"") << subspace->get(subspace->const_index, false)
                   << " != " << loop_idx;
    is_first = false;
  }

  // identity map subspace variable_iterators;
  for( Subspace::size_type i = 0; i < subspace->complete_size(); ++i ){
    transformation << " and " << subspace->get(i, true) << " = " << subspace->get(i, false);
  }
  transformation << "\n\t\t";
  // identity map next_subspace variable_iterators;
  for( Subspace::size_type i = 0; i < next_subspace->complete_size(); ++i ){
    transformation << " and " << next_subspace->get(i, true) << " = " << next_subspace->get(i, false);
  }

  transformation << "\n};";

  transformations.push_back( transformation.str() );

  return transformations;

}

FusionTransformation::iterator FusionTransformation::begin(){
  return this->fusion_loops.begin();
}

FusionTransformation::iterator FusionTransformation::end(){
  return this->fusion_loops.end();
}
