#ifndef ACCESSES_HPP
#define ACCESSES_HPP

#include <string>
#include <iostream>
#include <vector>
#include <set>

namespace LoopChainIR {

  class Tuple {
    private:
      typedef std::vector<int> container_type;
      container_type tuple;
    public:
      typedef container_type::size_type size_type;
      typedef container_type::iterator iterator;
      Tuple( std::vector<int> values );
      Tuple( const Tuple& other );

      static Tuple createMagicEmptyTuple();

      size_type dimensions() const;
      bool isEmptyTuple() const;
      iterator begin();
      iterator end();

      int operator[]( size_type index ) const;
      Tuple operator+( const Tuple& that ) const;
      Tuple operator-( const Tuple& that ) const;
      // Unary minus
      Tuple operator-( ) const;
      bool operator<( const Tuple& that ) const;
      bool operator>( const Tuple& that ) const;
      bool operator==( const Tuple& that ) const;
      bool operator!=( const Tuple& that ) const;

      std::string str() const;
      friend std::ostream& LoopChainIR::operator<<( std::ostream& os, const Tuple& tuple);
  };

  class TupleCollection {
    private:
      typedef std::set<Tuple> container_type;

      container_type tuples;
      const Tuple::size_type dimensions_var;

    public:
      typedef container_type::iterator iterator;

      TupleCollection( Tuple::size_type dimensions );
      TupleCollection( std::set<Tuple> tuples, signed int dimensions = -1 );
      TupleCollection( const TupleCollection& that );
      Tuple::size_type dimensions() const;
      iterator begin();
      iterator end();
      Tuple maxOnDims() const;
      Tuple minOnDims() const;
      void shiftAll( Tuple& extent );

      std::string str() const;
      friend std::ostream& LoopChainIR::operator<<( std::ostream& os, const TupleCollection& collection);
  };

  class Dataspace {
    public:
      const std::string name;
    private:
      const TupleCollection read_collection;
      const TupleCollection write_collection;
      Tuple::size_type dimensions_var;
    public:
      Dataspace( std::string name, std::set<Tuple> reads, std::set<Tuple> writes );
      Dataspace( std::string name, const TupleCollection& reads, const TupleCollection& writes );

      TupleCollection reads() const;
      TupleCollection writes() const;
      Tuple::size_type dimensions() const;

      std::string str() const;
      friend std::ostream& LoopChainIR::operator<<( std::ostream& os, const Dataspace& dataspace);
  };

}

#endif
