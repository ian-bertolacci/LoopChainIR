/*! ****************************************************************************
\file Subspace.hpp
\authors Ian J. Bertolacci

\brief
Defines classes required for representing, managing, and iterating over Subspaces

\copyright
Copyright 2015-2016 Colorado State University
Copyright 2017 Universiy of Arizona
*******************************************************************************/

#ifndef SUBSPACE_HPP
#define SUBSPACE_HPP

#include <vector>
#include <string>
#include <limits>
#include <map>

namespace LoopChainIR {

  // Forward declarations
  class Subspace;
  class SubspaceIterator;
  class ConstSubspaceIterator;
  class SubspaceManager;

  /*! \brief Type for counts of things not in containers. */
  typedef unsigned int count_type;

  /*! \brief Type for timestamps used to denote the stage of a Subspace. */
  typedef int timestamp_t;

  /*! \brief Common values for timestamp_t. */
  namespace timestamp_flags {
    /*! \brief The last (maximum) allowed timestamp_t. */
    static const timestamp_t AllStages = std::numeric_limits<timestamp_t>::max();
    /*! \brief The first (minimum) allowed timestamp_t. */
    static const timestamp_t BaseStage = 0;
    /*! \brief Denotes an unset timestamp_t. */
    static const timestamp_t Unstaged = -1;
  }

  /* \brief Encapsulates contiguous string of iterators used to form an iteration space.*/
  class Subspace {
    public:
        typedef std::vector<std::string> Container;
        typedef SubspaceIterator iterator;
        typedef ConstSubspaceIterator const_iterator;
        typedef Container::size_type size_type;

        /*! \brief Prefix used on iterators when in the alias state. */
        static const std::string alias_prefix; // alias_
        /*! \brief Suffix used to denote constant iterator from prefix. */
        static const std::string const_suffix; // _c

        /*! \brief The index to this Subspace's constant iterator */
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

      /*! \brief Returns an iterator to the beginning of the subspace, with option to use aliases. */
      iterator begin( bool use_aliases ) ;
      /*! \brief Returns an iterator to the beginning of the subspaces, respecting alias state. */
      iterator begin() ;
      /*! \brief Returns an iterator to the end of the subspaces. */
      iterator end() ;

      /*! \brief Returns a const_iterator to the beginning of the subspace, with option to use aliases. */
      const_iterator begin( bool use_aliases ) const;
      /*! \brief Returns a const_iterator to the beginning of the subspaces, respecting alias state. */
      const_iterator begin() const;
      /*! \brief Returns a const_iterator to the end of the subspaces. */
      const_iterator end() const;

      /*! \brief Returns the number of the variable iterators. */
      size_type size() const;
      /*! \brief Returnst the total number of iterators, both variable and constant */
      size_type complete_size() const;

      /*! \brief Set the Subspace to be in the aliased state. */
      void set_aliased();
      /*! \brief Set the subspace to be in the unset_aliased state. */
      void unset_aliased();
      /*! \brief Return true if the Subspace is in the aliased state. */
      bool is_aliased() const;

      /*! \brief Returns the stage assigned to this Subspace. */
      timestamp_t get_stage() const;
      /*! \brief Returns a string of the iterators at or before the specified stage, with option to use aliases. */
      std::string get_iterators( timestamp_t stage, bool use_aliases );

      /*! \brief Returns the index'th iterator, with option to use aliases. */
      std::string get( size_type index, bool use_aliases );

      /*! \brief Returns the index'th iterator, respecting alias state. */
      std::string operator[]( size_type index );
      /*! \brief Returns true if the subspaces have the same iterators. Aliasing and stage not considered. */
      bool operator==( const Subspace that ) const;
      /*! \brief Returns true if the subspaces do not have the same iterators. Aliasing and stage not considered. */
      bool operator!=( const Subspace that ) const;

      /*! \brief Assign the Subsapce a stage. */
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

  /*! \brief Manages the collection of Subspaces that form an iteration space. */
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

      std::map<std::string,int> safe_prefixes;

    public:
      SubspaceManager( Subspace* loop, Subspace* nest );

      /*! \brief Returns an iterator to the first Subspace, with option to use aliases. */
      iterator begin( bool use_aliases );
      /*! \brief Returns an iterator to the first Subspace, respecting alias state of Subspaces */
      iterator begin();
      /*! \brief Returns an iterator to the end of the Subspaces. */
      iterator end();
      /*! \brief Returns an itreator referencing the loops Subspace (typically [loops_c]). */
      iterator get_iterator_to_loops()
      /*! \brief Returns an itreator referencing the nests Subspace (typically [i_0,i_1,...m,i_c]). */;
      iterator get_iterator_to_nest();
      /*! \brief Given a pointer to a Subspace this SubspaceManager manages, returns an iterator to it. */
      iterator get_iterator_to_subspace( Subspace* subspace );
      /*! \brief Returns a pointer to the loops Subspace. */
      Subspace* get_loops();
      /*! \brief Returns a pointer to the nests Subspace. */
      Subspace* get_nest();

      std::string get_safe_prefix( std::string basic );

      iterator insert_left( Subspace* subpsace, iterator cursor );
      iterator insert_right( Subspace* subpsace, iterator cursor );

      Subspace::size_type size();

      /*! \brief Returns the current stage. */
      timestamp_t get_current_stage();
      /*! \brief Returns the stage that forms the input iteration space of a function created at this stage. */
      timestamp_t get_input_stage();
      /*! \brief Returns the stage that forms the output iteration space of a function created at this stage. */
      timestamp_t get_output_stage();
      /*! \brief Advances the curent stage forward by one. */
      timestamp_t advance_stage();
      /*! \brief Advances stage, and unsets alias for all managed subspaces. */
      timestamp_t next_stage();

      /*! \brief Returns string of iterators for this stage, with option to use aliases. */
      std::string get_iterators( timestamp_t stage, bool use_aliases );
      /*! \brief Returns string of iterators for the current stage, respecting all Subspaces alias state. */
      std::string get_iterators( );
      /*! \brief Returns string of iterators that forms the input iteration space of a function created at this stage, not respecting any Subspaces alias state. */
      std::string get_input_iterators();
      /*! \brief Returns string of iterators that forms the output iteration space of a function created at this stage, respecting all Subspaces alias state. */
      std::string get_output_iterators();
  };
}
#endif
