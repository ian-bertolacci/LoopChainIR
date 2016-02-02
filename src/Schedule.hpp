/*! ****************************************************************************
\file Schedule.hpp
\autors Ian J. Bertolacci

\brief
Defines the Schedule class

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/
#ifndef SCHEDULE_HPP
#define SCHEDULE_HPP

#include "LoopChain.hpp"
#include "util.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
/*!
\brief
Encapsulates a loop chain and its transformation schedules.
*/
class Schedule{
public:
  typedef std::vector<std::string>::iterator iterator;
  typedef std::vector<std::string>::const_iterator const_iterator;
  typedef std::vector<std::string>::size_type size_type;

private:
  LoopChain chain;
  int iterators_length;
  std::vector<std::string> transformations;
  std::vector<std::string> domains;

public:
  Schedule( LoopChain& chain );

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

public:
  friend std::ostream& operator<<( std::ostream& os, const Schedule& schedule);

};
/*!
\brief
print Schedule object to ostream.
*/
std::ostream& operator<<( std::ostream& os, const Schedule& schedule);

#endif
