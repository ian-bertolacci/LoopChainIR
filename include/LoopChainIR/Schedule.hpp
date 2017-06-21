/*! ****************************************************************************
\file Schedule.hpp
\authors Ian J. Bertolacci

\brief
Defines the Schedule class

\copyright
Copyright 2015-2016 Colorado State University
Copyright 2017 Universiy of Arizona
*******************************************************************************/
#ifndef SCHEDULE_HPP
#define SCHEDULE_HPP

#include <LoopChainIR/LoopChain.hpp>
#include <LoopChainIR/RectangularDomain.hpp>
#include <LoopChainIR/Transformation.hpp>
#include <LoopChainIR/IslAstRoot.hpp>
#include <LoopChainIR/all_isl.hpp>
#include <LoopChainIR/Subspace.hpp>
#include <LoopChainIR/util.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>


// Forward declarations because C++ was a mistake.
namespace LoopChainIR {
  class Transformation;
  class Schedule;

  /*!
  \brief
  print Schedule object to ostream.
  */
  std::ostream& operator<<( std::ostream& os, const Schedule& schedule);
}

namespace LoopChainIR {

  /*!
  \brief
  Encapsulates a loop chain and its transformation schedules.
  */
  class Schedule {
  public:
    typedef std::vector<std::string>::iterator iterator;
    typedef std::vector<std::string>::const_iterator const_iterator;
    typedef std::vector<std::string>::size_type size_type;

  private:
    LoopChain chain;
    RectangularDomain::size_type iterators_length;
    std::vector<std::string> transformations;
    std::vector<std::string> domains;
    std::string statement_prefix;
    std::string root_statement_symbol;
    std::string iterator_prefix;
    SubspaceManager manager;
    int depth;

    /*!
    \brief
    Appends an ISCC code string to transformations.

    \returns
    The index where the string was deposited.
    */
    size_type append( std::string text );

  public:
    Schedule( LoopChain& chain, std::string statement_prefix = std::string(""), std::string iterator_prefix = "c" );
    /*!
    \returns The length (in symbols) of the loop chain's iterator.
    */
    RectangularDomain::size_type getIteratorsLength();

    /*!
    \brief
    Changes the length (in symbols) of the loop chain's iterator by delta.

    \param[in] delta The change in length.

    \returns
    The new length.
    */
    RectangularDomain::size_type modifyIteratorsLength( int delta );


    /*!
    \brief returns the original loop chain
    */
    LoopChain& getChain();

    /*!
    \brief returns the original loop chain
    */

    /*!
    \brief
    Apply the Transformation to the schedule.

    */
    void apply( Transformation& scheduler );

    /*!
    \brief
    Apply the Transformation to the schedule.
    \param[in] schedulers An ordered list of Transformation objects that will be

    applied to the schedule object.
    */
    void apply( std::vector<Transformation*> schedulers );

    /*!
    \brief
    starting iterator over the domain strings
    */
    iterator begin_domains();

    /*!
    \brief
    ending iteration over the domain strings
    */
    iterator end_domains();

    /*!
    \brief
    starting iterator over the transformations strings
    */
    iterator begin_transformations();

    /*!
    \brief
    ending iterator over the transformations strings
    */
    iterator end_transformations();

    /*!
    \brief
    starting const iterator over the domain strings
    */
    const_iterator begin_domains() const ;

    /*!
    \brief
    ending const iteration over the domain strings
    */
    const_iterator end_domains() const ;

    /*!
    \brief
    starting const iterator over the transformations strings
    */
    const_iterator begin_transformations() const ;

    /*!
    \brief
    ending const iterator over the transformations strings
    */
    const_iterator end_transformations() const ;

    /*!
    \brief
    Transform the initial loop chain using the applied trasformations and
    generate the resulting loop code.

    \returns
    std::string of generated loop code.
    */
    std::string codegen( );

    /*!
    \brief
    Return copy of the statment prefix.

    \returns
    std::string copy of the statement prefix.
    */
    std::string getStatementPrefix();

    /*!
    \brief
    Returns copy of the root statement symbol.

    \returns
    std::string copy of the root statement symbol.
    */
    std::string getRootStatementSymbol();

    std::string getIteratorPrefix();

    /*!
    \brief
    Transform the initial loop chain using the applied trasformations and
    generate the resulting loop code to a file via ISL's code printer.

    \param[in] file_name Path to file being written.

    \returns
    bool true if stream good and not fail and not bad
    */
    bool codegenToFile( std::string file_name );

    /*!
    \brief
    Transform the initial loop chain using the applied trasformations and
    generate the resulting loop code to ISL AST.

    \returns
    Pointer to isl_ast_node struct.
    */
    IslAstRoot* codegenToIslAst();

    /*! \brief Generates ISCC code that can be used by the ISCC interpreter to generate code equivalent to the output of ISL */
    std::string codegenToISCC( ) const;

    /*! \brief Get a reference to the manager. */
    SubspaceManager& getSubspaceManager();

    /*! \brief Get the current depth of nested transformations. */
    int getDepth();

    /*! \brief Increase depth of nested transformations. */
    int incrementDepth();

    /*! \brief Decrease depth of nested transformations. */
    int decrementDepth();

  public:
    friend std::ostream& LoopChainIR::operator<<( std::ostream& os, const Schedule& schedule);

  };


}

#endif