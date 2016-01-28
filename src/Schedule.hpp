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

class Schedule{
public:
  typedef std::vector<std::string>::iterator iterator;
  typedef std::vector<std::string>::const_iterator const_iterator;

private:
  LoopChain chain;
  int iterators_length;
  std::vector<std::string> transformations;
  std::vector<std::string> domains;

public:
  Schedule( LoopChain& chain );

  iterator begin_domains();
  iterator end_domains();

  iterator begin_transformations();
  iterator end_transformations();


  const_iterator begin_domains() const ;
  const_iterator end_domains() const ;

  const_iterator begin_transformations() const ;
  const_iterator end_transformations() const ;

  std::string codegen( );

public:
  friend std::ostream& operator<<( std::ostream& os, const Schedule& schedule);

};

std::ostream& operator<<( std::ostream& os, const Schedule& schedule);

#endif
