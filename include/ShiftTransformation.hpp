/*! ****************************************************************************
\file ShiftTransformation.hpp
\authors Ian J. Bertolacci

\brief
Shift a single loop nest by some extent

\copyright
Copyright 2015-2016 Colorado State University
Copyright 2017 Universiy of Arizona
*******************************************************************************/

#ifndef SHIFT_TRANSFORMATION_HPP
#define SHIFT_TRANSFORMATION_HPP

#include "LoopChain.hpp"
#include "Transformation.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "stdio.h"

namespace LoopChainIR {


  class ShiftTransformation : public Transformation {
  private:
    LoopChain::size_type loop_id;
    std::vector<std::string> extents;
    std::vector<std::string> symbols;

  public:
    typedef int size_type;

    /*!
    \brief
    Shift all loops specified in nest by the extents.
    \param[in] loop The id of the loop-nest within the chain to be shifted
    \param[in] extent The extent to shift the loop by.
               Note that extents.dimensions() must be equal to
               chain.getNest( loop ).getDomain().dimensions(), where chain is the
               LoopChain on which the transformations are being applied to.
    */
    ShiftTransformation( LoopChain::size_type loop, Tuple extent );

    /*!
    \brief
    Shift all loops specified in nest by the extents.
    \param[in] loop The id of the loop-nest within the chain to be shifted
    \param[in] extents The extent by which the loop is shifted by.
               Note that extents.size() must be equal to
               chain.getNest( loop ).getDomain().dimensions(), where chain is the
               LoopChain on which the transformations are being applied to.
    */
    ShiftTransformation( LoopChain::size_type loop, std::vector<std::string> extents );

    /*!
    \brief
    Shift all loops specified in nest by the extents.
    \param[in] loop The id of the loop-nest within the chain to be shifted
    \param[in] extents The extent by which the loop is shifted by.
               Note that extents.size() must be equal to
               chain.getNest( loop ).getDomain().dimensions(), where chain is the
               LoopChain on which the transformations are being applied to.
    \param[in] symbols Symbols found within any of the extent expressions.
    */
    ShiftTransformation( LoopChain::size_type loop, std::vector<std::string> extents, std::vector<std::string> symbols );

    /*!
    \brief
    Shift all loops specified in nest by the extent.
    \param[in] loop The id of the loop-nest within the chain to be shifted
    \param[in] extent The extent by which the loops are shifted by
    */
    ShiftTransformation( LoopChain::size_type loop, std::string extent );

    /*!
    \brief
    Shift all loops specified in nest by the extent.
    \param[in] loop The id of the loop-nest within the chain to be shifted
    \param[in] extent The extent by which the loops are shifted by
    \param[in] symbols Symbols found within the extent expression.
    */
    ShiftTransformation( LoopChain::size_type loop, std::string extent, std::vector<std::string> symbols );

    /*!
    \returns The loop id within the associated loop chain.
    */
    LoopChain::size_type getLoopId();

    /*!
    \returns A vector of the extents;
    */
    std::vector<std::string> getExtents();

    /*!
    \returns A vector of the symbols within the extents;
    */
    std::vector<std::string> getSymbols();

    /*!
    \brief
    Generate ISCC code for the shift transformation, and append it to the
    transformation list of schedule (modifies schedule).

    \param[inout] schedule Schedule this transformation is being applied to.

    \returns
    The ISCC code as a string
    */
    std::vector<std::string> apply( Schedule& schedule, Subspace* subspace );

    /*!
    \brief
    Generate ISCC code for the shift transformation, and append it to the
    transformation list of schedule (modifies schedule).

    \param[inout] schedule Schedule this transformation is being applied to.

    \returns
    The ISCC code as a string
    */
    std::vector<std::string> apply( Schedule& schedule );
  };

  class DataspaceMinMax {
    public:
      const std::string name;
    private:
      Tuple maxReads_tuple,
            minReads_tuple,
            maxWrites_tuple,
            minWrites_tuple;
    public:
      DataspaceMinMax( const Dataspace& dataspace );
      DataspaceMinMax( const DataspaceMinMax& that );

      Tuple maxRead() const;
      Tuple minRead() const;
      Tuple maxWrite() const;
      Tuple minWrite() const;

      void postShiftUpdate( const Tuple& extent );
      void selfUnion( const DataspaceMinMax& dataspace_minmax );
      void selfUnion( const Dataspace& dataspace );

      std::string str() const;
      friend std::ostream& LoopChainIR::operator<<( std::ostream& os, const DataspaceMinMax& dataspace );
  };

  class DataspaceMinMaxCollection {
    private:
      std::map< std::string, DataspaceMinMax > dataspaces;
    public:
      typedef std::map< std::string, DataspaceMinMax >::iterator iterator;
      DataspaceMinMaxCollection( );
      DataspaceMinMaxCollection( std::list<DataspaceMinMax> dataspaces );
      DataspaceMinMaxCollection( std::list<Dataspace> dataspaces );
      void postShiftUpdate( const Tuple& extent );
      void selfUnion( const DataspaceMinMax& dataspace_minmax );
      void selfUnion( const Dataspace& dataspace );

      iterator begin();
      iterator end();
      bool contains( std::string name ) const;
      DataspaceMinMax& operator[]( std::string name );
      const DataspaceMinMax& operator[]( std::string name ) const;

      std::string str() const;
      friend std::ostream& LoopChainIR::operator<<( std::ostream& os, const DataspaceMinMaxCollection& collection );
  };

  std::list<Tuple> computeShiftTuplesForFusion( const LoopChain& chain );

  std::list<ShiftTransformation*> computeShiftForFusion( const LoopChain& chain );

}

#endif
