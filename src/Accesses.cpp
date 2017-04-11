/*! ****************************************************************************
\file Accesses.cpp
\authors Ian J. Bertolacci

\brief
Classes to record regular accesses into a dataspace

\copyright
Copyright 2015-2016 Colorado State University
Copyright 2017 Universiy of Arizona
*******************************************************************************/

#include <limits>
#include <sstream>

#include "Accesses.hpp"
#include "util.hpp"

using namespace std;
using namespace LoopChainIR;

Tuple::Tuple( vector<int> values )
: tuple( values.begin(), values.end() )
{ }

Tuple::Tuple( const Tuple& that )
: Tuple( that.tuple )
{ }

Tuple Tuple::createMagicEmptyTuple( ){
  return Tuple( vector<int>() );
}

Tuple::size_type Tuple::dimensions() const {
  return this->tuple.size();
}

bool Tuple::isEmptyTuple() const {
  return this->dimensions() == 0;
}

Tuple::iterator Tuple::begin(){
  return this->tuple.begin();
}

Tuple::iterator Tuple::end(){
  return this->tuple.end();
}

int Tuple::operator[]( Tuple::size_type index ) const {
  return this->tuple[index];
}

Tuple Tuple::operator+( const Tuple& that ) const {
  assertWithException( this->dimensions() == that.dimensions(),
                       SSTR( "Tuples are of different dimensionality "
                             << this->dimensions() << " "
                             << that.dimensions() ) );

  vector<int> result( this->dimensions(), 0 );
  for( size_type i = 0; i < this->dimensions(); ++i ){
    result[i] = (*this)[i] + that[i];
  }

  return  Tuple(result);
}

Tuple Tuple::operator-( const Tuple& that ) const {
  assertWithException( this->dimensions() == that.dimensions(),
                       SSTR( "Tuples are of different dimensionality "
                             << this->dimensions() << " "
                             << that.dimensions() ) );

  vector<int> result( this->dimensions(), 0 );
  for( size_type i = 0; i < this->dimensions(); ++i ){
    result[i] = (*this)[i] - that[i];
  }

  return Tuple(result);
}

Tuple Tuple::operator-( ) const {
  vector<int> result( this->dimensions(), 0 );

  for( size_type i = 0; i < this->dimensions(); ++i ){
    result[i] = - (*this)[i];
  }

  return Tuple(result);
}

bool Tuple::operator<( const Tuple& that ) const {
  if( this->dimensions() < that.dimensions() ){
    return true;
  } else if( this->dimensions() > that.dimensions() ){
    return false;
  }

  for( size_type i = 0; i < this->dimensions(); ++i ){
    if( (*this)[i] == that[i] ){
      continue;
    } else if( (*this)[i] < that[i] ){
      return true;
    } else {
      return false;
    }
  }

  return false;
}

bool Tuple::operator>( const Tuple& that ) const {
  return that < *this;
}

bool Tuple::operator==( const Tuple& that ) const {
  if( this->dimensions() != that.dimensions() ){
    return false;
  }

  for( size_type i = 0; i < this->dimensions(); ++i ){
    if( (*this)[i] != that[i] ){
      return false;
    }
  }

  return true;
}

bool Tuple::operator!=( const Tuple& that ) const {
  return !( this->operator==( that ) );
}

std::string Tuple::str() const {
  ostringstream stream;
  stream << "(";
  bool first = true;
  for( int element : this->tuple ){
    if( !first ){
      stream << ", ";
    } else {
      first = false;
    }
    stream << element;
  }
  stream << ")";
  return stream.str();
}

std::ostream& LoopChainIR::operator<<( std::ostream& os, const Tuple& tuple){
  return (os << tuple.str() );
}

TupleCollection::TupleCollection( Tuple::size_type dimensions )
: tuples( ), dimensions_var( dimensions )
{ }

TupleCollection::TupleCollection( std::set<Tuple> tuples, signed int dimensions )
: tuples( (tuples.erase( Tuple::createMagicEmptyTuple() ), tuples) ), dimensions_var( (tuples.size() > 0)? tuples.begin()->dimensions() : (Tuple::size_type) dimensions )
{
  // assert that if there given an empty collection, a dimensionality was given
  assertWithException( this->tuples.size() > 0 || (this->tuples.size() == 0 && this->dimensions() > 0),
                       "Dimensionality must be specified for collection with no initial set." );

  for( Tuple tuple : this->tuples ){
    assertWithException( this->dimensions() == tuple.dimensions(), "Not all tuples are of the same dimensionality" );
  }
}

TupleCollection::TupleCollection( const TupleCollection& that )
: TupleCollection( that.tuples, that.dimensions_var )
{ }

TupleCollection::TupleCollection( const TupleCollection& left, const TupleCollection& right )
: TupleCollection( []( std::set<Tuple> s_left, std::set<Tuple> s_right ){
                      std::set<Tuple> tmp( s_left );
                      tmp.insert( s_right.begin(), s_right.end() );
                      return tmp;
                    }( left.tuples, right.tuples ),
                    left.dimensions() | right.dimensions()
                  )
{ }


Tuple::size_type TupleCollection::dimensions() const {
  return this->dimensions_var;
}

TupleCollection::iterator TupleCollection::begin(){
  return this->tuples.begin();
}

TupleCollection::iterator TupleCollection::end(){
  return this->tuples.end();
}

Tuple TupleCollection::maxOnDims() const {
  if( this->tuples.size() < 1 ){
    return Tuple::createMagicEmptyTuple();
  }

  vector<int> max_values( this->dimensions(), numeric_limits<int>::min() );

  for( Tuple tuple : this->tuples ){
    for( Tuple::size_type d = 0; d < tuple.dimensions(); d += 1 ){
      max_values[d] = max( max_values[d], tuple[d] );
    }
  }

  return Tuple(max_values);
}

Tuple TupleCollection::minOnDims() const {
  if( this->tuples.size() < 1 ){
    return Tuple::createMagicEmptyTuple();
  }

  vector<int> min_values( this->dimensions(), numeric_limits<int>::max() );

  for( Tuple tuple : this->tuples ){
    for( Tuple::size_type d = 0; d < tuple.dimensions(); d += 1 ){
      min_values[d] = min( min_values[d], tuple[d] );
    }
  }

  return Tuple(min_values);
}

void TupleCollection::shiftAll( Tuple& extent ){
  assertWithException( extent.dimensions() == this->dimensions(),
                       SSTR( "Shift extent dimensionality (" << extent.dimensions()
                             << ") than collection (" << this->dimensions() << ")" ) );
  set<Tuple> new_set;
  for( Tuple tuple : this->tuples ){
    new_set.insert( tuple + extent );
  }
  this->tuples = new_set;
}

std::string TupleCollection::str( ) const {
  ostringstream stream;
  stream << "{ ";
  bool first = true;
  for( Tuple tuple : this->tuples ){
    if( !first ){
      stream << ", ";
    } else {
      first = false;
    }
    stream << tuple.str();
  }
  stream << " }";
  return stream.str();
}

std::ostream& LoopChainIR::operator<<( std::ostream& os, const TupleCollection& collection ){
  return (os << collection.str() );
}

Dataspace::Dataspace( std::string name, std::set<Tuple> reads, std::set<Tuple> writes )
: name( name ), read_collection( reads ), write_collection( writes )
{
  assertWithException( read_collection.dimensions() == write_collection.dimensions(), "Read/Write sets are of different dimensionality" );
  this->dimensions_var = read_collection.dimensions();
}

Dataspace::Dataspace( std::string name, const TupleCollection& reads, const TupleCollection& writes )
: name( name ), read_collection( reads ), write_collection( writes ), dimensions_var( reads.dimensions() )
{
  assertWithException( read_collection.dimensions() == write_collection.dimensions(), "Read/Write sets are of different dimensionality" );
}

TupleCollection Dataspace::reads() const {
  return TupleCollection( this->read_collection );
}

TupleCollection Dataspace::writes() const {
  return TupleCollection( this->write_collection );
}

Tuple::size_type Dataspace::dimensions() const {
  return this->dimensions_var;
}

std::string Dataspace::str() const {
  ostringstream stream;
  stream << this->name << ": "
         << "\n\tReads: " << this->read_collection
         << "\n\tWrites: " << this->write_collection;
  return stream.str();
}

std::ostream& LoopChainIR::operator<<( std::ostream& os, const Dataspace& dataspace ){
  return (os << dataspace.str() );
}
