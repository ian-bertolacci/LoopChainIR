/*! ****************************************************************************
\file TileTransformation.cpp
\autors Ian J. Bertolacci

\brief
Tile a loop nest

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/

#include "TileTransformation.hpp"
#include "DefaultSequentialTransformation.hpp"
#include <iostream>
#include <sstream>
#include <iostream>

using namespace LoopChainIR;

int TileTransformation::num_prefixes_used = 0;

TileTransformation::TileTransformation( LoopChain::size_type loop, TileMap tile_sizes, Transformation* over_tiles, Transformation* within_tiles )
: loop(loop), tile_sizes( tile_sizes ), uniform( false ), over_tiles( over_tiles ), within_tiles( within_tiles )
{
  assertWithException( tile_sizes.size() > 0, "Must tile along one or more dimensions." );
}

/*
TileTransformation::TileTransformation( LoopChain::size_type loop, TileTransformation::mapped_type tile_size )
  : loop(loop), uniform(true), uniform_size(tile_size) {

}
*/

TileTransformation::TileTransformation( LoopChain::size_type loop, TileTransformation::TileMap tile_sizes )
: TileTransformation( loop, tile_sizes, new DefaultSequentialTransformation(), new DefaultSequentialTransformation() )
{ }


bool TileTransformation::isUniformSize( ){
  return this->uniform;
}

TileTransformation::mapped_type TileTransformation::getSize( TileTransformation::key_type i ){
  return this->isUniformSize() ? this->uniform_size : this->tile_sizes[ i ];
}

TileTransformation::TileMap TileTransformation::getSizes(){
  return std::map<TileTransformation::key_type, TileTransformation::mapped_type>( this->tile_sizes );
}

std::vector<std::string> TileTransformation::apply( Schedule& schedule){
  return this->apply( schedule, schedule.getSubspaceManager().get_nest() );
}

std::vector<std::string> TileTransformation::apply( Schedule& schedule, Subspace* subspace ){

  assertWithException( this->tile_sizes.size() <= subspace->size(), "Tiling more dimensions than exist in the subspace." );

  std::vector<std::string> transformations;
  std::ostringstream transformation;

  SubspaceManager& manager = schedule.getSubspaceManager();
  SubspaceManager::iterator subspace_cursor = manager.get_iterator_to_subspace( subspace );
  Subspace* loops = manager.get_loops();

  std::ostringstream tile_prefix;
  for( int i = 0; i <= schedule.getDepth(); i += 1 ){
    tile_prefix << "t";
  }

  // We will remake this subspace
  Subspace* tile_subspace = new Subspace( tile_prefix.str(), (this->uniform?subspace->size():this->tile_sizes.size()), *subspace );

  // to support mutli-loop and loop exclusionary tiling, check for existing
  // tiling subspace and use that
  // if there is not an existing tiling subspace

  bool found_previous_tile_subspace = false;
  SubspaceManager::iterator finding_cursor = subspace_cursor;
  // First off, cannot search previous subspaces if we dont have any.
  if( subspace_cursor != manager.begin() ){
    bool finished_loop = false;
    while( !finished_loop && **finding_cursor != *tile_subspace ){
      if( finding_cursor == manager.begin() ){
        finished_loop = true;
      } else {
        finding_cursor = std::prev( finding_cursor );
      }
    }
    found_previous_tile_subspace = !finished_loop;
  }

  // If we did find one: use it
  if( found_previous_tile_subspace ){
    tile_subspace = *finding_cursor;
    tile_subspace->set_aliased();
  }
  // If we didn't then insert the created one.
  else{
    tile_subspace = new Subspace( manager.get_safe_prefix("t"), (this->uniform?subspace->size():this->tile_sizes.size()), *subspace );
    manager.insert_left( tile_subspace, subspace_cursor );
  }

  subspace->set_aliased();

  transformation << "{\n\t [" << manager.get_input_iterators() << "] -> ["
                 << manager.get_output_iterators() << "] : \n\t\t"
                 << loops->get( loops->const_index, false ) << " = "
                 << this->loop << " and "
                 << subspace->get( subspace->const_index, true ) << " = "
                 << subspace->get( subspace->const_index, false ) << " and "
                 << tile_subspace->get( tile_subspace->const_index, true )
                 << " = 0";

  for( Subspace::size_type i = 0; i < subspace->size(); ++i ){
    if( i < tile_subspace->size() ){
      auto tile_size = this->getSize((key_type) i+1);
      transformation << " and\n\t\t" << tile_subspace->get(i,true) << " * "
                     << tile_size << " <= " << subspace->get( i, false )
                     << " < (" << tile_subspace->get(i,true) << " + 1 ) * "
                     << tile_size;
    }
    transformation << " and " << subspace->get( i, true ) << " = "
                   << subspace->get( i, false );
  }

  transformation << ";\n\t";

  // identity map unmapped loops
  subspace->unset_aliased();
  tile_subspace->unset_aliased();
  transformation << "[" << manager.get_input_iterators() << "] -> ["
                 << manager.get_output_iterators() << "] : \n\t\t"
                 << loops->get( loops->const_index, false ) << " != "
                 << this->loop;

  if( !found_previous_tile_subspace ){
    for( Subspace::size_type i = 0; i < tile_subspace->complete_size(); ++i ){
      transformation << " and\n\t\t" << tile_subspace->get( i, true ) << " = 0";
    }
  }

  transformation << ";\n};";

  //std::cout << transformation.str() << std::endl;

  transformations.push_back( transformation.str() );

  // Apply over tile transformation on the tile subpsace
  manager.next_stage();
  schedule.incrementDepth();
  std::vector<std::string> over_transformations = this->over_tiles->apply( schedule, tile_subspace );

  // Apply within tile transformation on the tiled subspace
  manager.next_stage();
  std::vector<std::string> within_transformations = this->within_tiles->apply( schedule, subspace );
  schedule.decrementDepth();

  // append (in order) the the over and within transformations
  transformations.insert( transformations.end(), over_transformations.begin(), over_transformations.end() );
  transformations.insert( transformations.end(), within_transformations.begin(), within_transformations.end() );

  manager.next_stage();
  return transformations;
}
