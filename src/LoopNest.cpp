/*! ****************************************************************************
\file LoopNest.cpp
\authors Ian J. Bertolacci

\brief
Contains a set of bounds and and the access pattern of a single, arbitarily deep
loop nest.

\copyright
Copyright 2015-2016 Colorado State University
*******************************************************************************/

#include <LoopChainIR/LoopNest.hpp>
#include <LoopChainIR/util.hpp>

using namespace LoopChainIR;

LoopNest::LoopNest( RectangularDomain loop_bounds )
: bounds( loop_bounds ), dataspaces( )
{ }

LoopNest::LoopNest( RectangularDomain loop_bounds, std::list<Dataspace> dataspaces )
: bounds( loop_bounds ), dataspaces( dataspaces )
{ }

RectangularDomain& LoopNest::getDomain(){
  return this->bounds;
}

std::list<Dataspace> LoopNest::getDataspaces() const {
  return std::list<Dataspace>( this->dataspaces );
}

void LoopNest::replaceDataspaces( std::list<Dataspace> dataspaces ) {
  this->dataspaces.clear();
  this->dataspaces.insert( this->dataspaces.begin(), dataspaces.begin(), dataspaces.end() );
}

void LoopNest::shiftDataspaces( Tuple extent ){
  std::list<Dataspace> shifted_dataspaces;

  for( Dataspace& dataspace : this->dataspaces ){
    TupleCollection reads = dataspace.reads();
    TupleCollection writes = dataspace.writes();
    reads.shiftAll( extent );
    writes.shiftAll( extent );
    shifted_dataspaces.push_back( Dataspace( dataspace.name, reads, writes ) );
  }

  this->replaceDataspaces( shifted_dataspaces );
}
