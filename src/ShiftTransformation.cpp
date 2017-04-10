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
: loop_id( loop ),
  extents( []( Tuple in ){
        vector<string> _tmp( in.dimensions(), "" );
        transform( in.begin(), in.end(),
                   _tmp.begin(),
                   [](int i){ return to_string( i ); }
               );
        return _tmp;
      }( extent ) )
{ }

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


DataspaceMinMax::DataspaceMinMax( const Dataspace& dataspace )
: name( dataspace.name ),
  maxReads_tuple(  dataspace.reads().maxOnDims() ),  minReads_tuple(  dataspace.reads().minOnDims()  ),
  maxWrites_tuple( dataspace.writes().maxOnDims() ), minWrites_tuple( dataspace.writes().minOnDims() )
{ }

DataspaceMinMax::DataspaceMinMax( const DataspaceMinMax& that )
: name( that.name ),
  maxReads_tuple(  that.maxReads_tuple ),  minReads_tuple(  that.minReads_tuple  ),
  maxWrites_tuple( that.maxWrites_tuple ), minWrites_tuple( that.minWrites_tuple )
{ }

Tuple DataspaceMinMax::maxRead() const {
  return this->maxReads_tuple;
}

Tuple DataspaceMinMax::minRead() const {
  return this->minReads_tuple;
}

Tuple DataspaceMinMax::maxWrite() const {
  return this->maxWrites_tuple;
}

Tuple DataspaceMinMax::minWrite() const {
  return this->minWrites_tuple;
}

void DataspaceMinMax::postShiftUpdate( const Tuple& extent ){
  if( this->maxReads_tuple != Tuple::createMagicEmptyTuple() )
    this->maxReads_tuple = this->maxReads_tuple - extent;
  if( this->minReads_tuple != Tuple::createMagicEmptyTuple() )
    this->minReads_tuple = this->minReads_tuple - extent;
  if( this->maxWrites_tuple != Tuple::createMagicEmptyTuple() )
    this->maxWrites_tuple = this->maxWrites_tuple - extent;
  if( this->minWrites_tuple != Tuple::createMagicEmptyTuple() )
    this->minWrites_tuple = this->minWrites_tuple - extent;
}

void DataspaceMinMax::selfUnion( const DataspaceMinMax& dataspace_minmax ){
  if( dataspace_minmax.name == this->name ){
    this->maxReads_tuple = TupleCollection( { this->maxRead(), dataspace_minmax.maxRead() } ).maxOnDims();
    this->minReads_tuple = TupleCollection( { this->minRead(), dataspace_minmax.minRead() } ).minOnDims();
    this->maxWrites_tuple = TupleCollection( { this->maxWrite(), dataspace_minmax.maxWrite() } ).maxOnDims();
    this->minWrites_tuple = TupleCollection( { this->minWrite(), dataspace_minmax.minWrite() } ).minOnDims();
  }
}

void DataspaceMinMax::selfUnion( const Dataspace& dataspace ){
  this->selfUnion( DataspaceMinMax(dataspace) );
}

std::string DataspaceMinMax::str() const {
  ostringstream stream;
  stream << this->name << ": "
         << "\n\tMaxRead: " << this->maxRead()
         << "\n\tMinRead: " << this->minRead()
         << "\n\tMaxWrite: " << this->maxWrite()
         << "\n\tMinWrite: " << this->minWrite();
  return stream.str();
}

std::ostream& LoopChainIR::operator<<( std::ostream& os, const DataspaceMinMax& dataspace ){
  return (os << dataspace.str());
}

DataspaceMinMaxCollection::DataspaceMinMaxCollection( )
: dataspaces( )
{ }

DataspaceMinMaxCollection::DataspaceMinMaxCollection( std::list<DataspaceMinMax> dataspaces )
: dataspaces( )
{
  for( DataspaceMinMax dsmm : dataspaces ){
    this->selfUnion( dsmm );
  }
}

DataspaceMinMaxCollection::DataspaceMinMaxCollection( std::list<Dataspace> dataspaces )
: dataspaces( )
{
  for( Dataspace dataspace : dataspaces ){
    this->selfUnion( dataspace );
  }
}

void DataspaceMinMaxCollection::postShiftUpdate( const Tuple& extent ){
  for( pair< string, DataspaceMinMax > key_value : this->dataspaces ){
    this->dataspaces.find(key_value.first)->second.postShiftUpdate( extent );
  }
}

void DataspaceMinMaxCollection::selfUnion( const DataspaceMinMax& dataspace_minmax ){
  if( this->dataspaces.count( dataspace_minmax.name ) > 0 ){
    this->dataspaces.find( dataspace_minmax.name )->second.selfUnion( dataspace_minmax );
  } else {
    this->dataspaces.emplace( dataspace_minmax.name, DataspaceMinMax( dataspace_minmax ) );
  }
}

void DataspaceMinMaxCollection::selfUnion( const Dataspace& dataspace ){
  this->selfUnion( DataspaceMinMax( dataspace ) );
}

DataspaceMinMaxCollection::iterator DataspaceMinMaxCollection::begin(){
  return this->dataspaces.begin();
}

DataspaceMinMaxCollection::iterator DataspaceMinMaxCollection::end(){
  return this->dataspaces.end();
}

bool DataspaceMinMaxCollection::contains( std::string name ) const {
  return this->dataspaces.count( name ) > 0;
}

DataspaceMinMax& DataspaceMinMaxCollection::operator[]( std::string name ) {
  assertWithException( this->contains(name), SSTR( "No such dataspace " << name << " in collection" ) );
  return this->dataspaces.find( name )->second;
}

const DataspaceMinMax& DataspaceMinMaxCollection::operator[]( std::string name ) const {
  return this->dataspaces.find( name )->second;
}


std::string DataspaceMinMaxCollection::str() const {
  ostringstream stream;
  for( pair< string, DataspaceMinMax > key_value : this->dataspaces ){
    stream << key_value.second << endl;
  }
  return stream.str();
}

std::ostream& LoopChainIR::operator<<( std::ostream& os, const DataspaceMinMaxCollection& collection ){
  return (os << collection.str());
}

std::list<Tuple> LoopChainIR::computeShiftTuplesForFusion( const LoopChain& chain ){
  std::list<Tuple> shifts;

  DataspaceMinMaxCollection min_max_collection;
  Tuple::size_type dimensionality = 0;
  // Collect dimensionality of loop-chain dataspaces accesses
  {
    set<Tuple::size_type> dimensions_in_chain;
    for( LoopNest nest : chain ){
      for( Dataspace dataspace : nest.getDataspaces() ){
        dimensions_in_chain.insert( dataspace.dimensions() );
      }
    }
    assertWithException( dimensions_in_chain.size() <= 1, "There are dataspaces of different dimensionality in the loopchain" );
    // If there are no dimensions, then there are no dataspaces,
    // and no need to continue.
    if( dimensions_in_chain.empty() ){
      return shifts;
    }
    dimensionality = *(dimensions_in_chain.begin());
  }

  bool is_first_nest = true;
  for( LoopNest nest : chain ){
    Tuple shift = Tuple( vector<int>(dimensionality,0) );

    if( ! is_first_nest ){
      set<Tuple> shifts_of_all_data_spaces;

      for( Dataspace dataspace : nest.getDataspaces() ){
        if( ! min_max_collection.contains( dataspace.name ) ){
          continue;
        }

        const DataspaceMinMax pev_min_max = min_max_collection[dataspace.name];
        DataspaceMinMax current_min_max( dataspace );
        // Lineup all pairs
        list< pair<Tuple,Tuple> > pairs =
          {
            make_pair( current_min_max.maxWrite(), pev_min_max.minWrite() ),
            make_pair( current_min_max.maxRead(),  pev_min_max.minWrite() ),
            make_pair( current_min_max.maxWrite(), pev_min_max.minRead() ),
          };
        // remove pairs with any empty tuples
        pairs.erase( remove_if( pairs.begin(),
                                pairs.end(),
                                [](pair<Tuple,Tuple> x){ return x.first == Tuple::createMagicEmptyTuple()
                                                             || x.second == Tuple::createMagicEmptyTuple(); }),
                     pairs.end() );
        // perform subtraction
        for( pair<Tuple,Tuple> twople : pairs ){
          shifts_of_all_data_spaces.insert( twople.first - twople.second );
        }
      } // for dataspace : nest
      // If any shifting was neccessary, shift = maxOnDims of all shifts
      if( shifts_of_all_data_spaces.size() > 0 ){
        shift = TupleCollection( shifts_of_all_data_spaces ).maxOnDims();
      }
    } // if not first dataspace

    for( Dataspace dataspace : nest.getDataspaces() ){
      DataspaceMinMax min_max( dataspace );
      min_max.postShiftUpdate( shift );
      min_max_collection.selfUnion( min_max );
    }

    shifts.push_back( shift );

    is_first_nest = false;
  }

  return shifts;
}

std::list<ShiftTransformation*> LoopChainIR::computeShiftForFusion( const LoopChain& chain ){
  std::list<ShiftTransformation*> transformations;

  std::list<Tuple> extents = computeShiftTuplesForFusion( chain );
  assertWithException(  extents.size() == chain.length(),
                        SSTR( "Number of shift extents, " << extents.size()
                              << " is not equal to the number of nests in the loop chain, "
                              << chain.length() ) );
  {
    LoopChain::size_type id = 0;
    std::list<Tuple>::iterator tuple_it = extents.begin();
    while( id != chain.length() && tuple_it != extents.end() ){
      transformations.push_back( new ShiftTransformation( id, *tuple_it) );

      ++id;
      ++tuple_it;
    }
  }

  return transformations;
}
