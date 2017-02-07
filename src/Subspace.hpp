#ifndef SUBSPACE_HPP
#define SUBSPACE_HPP

#include <vector>
#include <string>
#include <limits>

namespace LoopChainIR {

  class Subspace;
  class SubspaceIterator;
  class ConstSubspaceIterator;
  class SubspaceManager;

  typedef unsigned int count_type;
  typedef int timestamp_t;

  namespace timestamp_flags {
    static const timestamp_t AllStages = std::numeric_limits<timestamp_t>::max();
    static const timestamp_t BaseStage = 0;
    static const timestamp_t Unstaged = -1;
  }

  class Subspace {
    public:
        typedef std::vector<std::string> Container;
        typedef SubspaceIterator iterator;
        typedef ConstSubspaceIterator const_iterator;
        typedef Container::size_type size_type;

        static const std::string alias_prefix; // alias_
        static const std::string const_suffix; // _c

        const size_type const_index;

    protected:
      Container variable_iterators;
      std::string constant_iterator;
      Container all_iterators;
      timestamp_t stage;
      bool is_currently_aliased;

    public:
      Subspace( std::string var_iter_prefix, count_type var_iter_count );
      Subspace( std::string prefix, count_type count, Subspace& that );

      iterator begin( bool use_aliases ) ;
      iterator begin() ;
      iterator end() ;

      const_iterator begin( bool use_aliases ) const;
      const_iterator begin() const;
      const_iterator end() const;

      size_type size() const;
      size_type complete_size() const;

      void set_aliased();
      void unset_aliased();
      bool is_aliased() const;

      timestamp_t get_stage() const;
      std::string get_iterators( timestamp_t stage, bool use_aliases );

      std::string operator[]( size_type index );
      bool operator==( const Subspace that ) const;
      bool operator!=( const Subspace that ) const;

      void set_stage( timestamp_t stage );
  };

  class SubspaceIterator : std::iterator<std::forward_iterator_tag, Subspace::Container::value_type> {
    public:
      typedef Subspace::Container::value_type value_type;
      typedef Subspace::Container::iterator base_iterator;

    private:
      const Subspace* subspace;
      base_iterator internal_iter;
      const bool use_aliases;

    public:
      SubspaceIterator( const Subspace* subspace, base_iterator iter, bool use_aliases );
      bool operator==( const SubspaceIterator that ) const;
      bool operator!=( const SubspaceIterator that ) const;
      value_type operator*();
      SubspaceIterator& operator++();
      SubspaceIterator& operator++( int );
      base_iterator get_iterator();
  };

  class ConstSubspaceIterator : std::iterator<std::forward_iterator_tag, Subspace::Container::value_type> {
    public:
      typedef Subspace::Container::value_type value_type;
      typedef Subspace::Container::const_iterator base_iterator;

    private:
      const Subspace* subspace;
      base_iterator internal_iter;
      const bool use_aliases;

    public:
      ConstSubspaceIterator( const Subspace* subspace, base_iterator iter, bool use_aliases );
      bool operator==( const ConstSubspaceIterator that ) const;
      bool operator!=( const ConstSubspaceIterator that ) const;
      value_type operator*();
      ConstSubspaceIterator& operator++();
      ConstSubspaceIterator& operator++( int );
      base_iterator get_iterator();
  };

  class SubspaceManager {
    public:
      typedef std::vector<Subspace*> Container;
      typedef Container::iterator iterator;

    protected:
      typedef std::insert_iterator<Container> insert_iterator;

      std::vector< Subspace* > subspaces;
      Subspace* loop;
      Subspace* nest;
      timestamp_t stage;

    public:
      SubspaceManager( Subspace* loop, Subspace* nest );

      iterator begin( bool use_aliases );
      iterator begin();
      iterator end();
      iterator get_cursor_to_loops();
      iterator get_cursor_to_nest();
      iterator get_cursor_to_subspace( Subspace* subspace );
      Subspace* get_loops();
      Subspace* get_nest();

      iterator insert_left( Subspace* subpsace, iterator cursor );
      iterator insert_right( Subspace* subpsace, iterator cursor );

      timestamp_t get_current_stage();
      timestamp_t get_input_stage();
      timestamp_t get_output_stage();
      timestamp_t advance_stage();
      timestamp_t next_stage();

      std::string get_iterators( timestamp_t stage, bool use_aliases );
      std::string get_iterators( );
      std::string get_input_iterators();
      std::string get_output_iterators();

  };
}
#endif
