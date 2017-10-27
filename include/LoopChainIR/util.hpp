/*! ****************************************************************************
\file util.hpp
\authors Ian J. Bertolacci

\brief
Contains a number of utility fuctions.

\copyright
Copyright 2015-2016 Colorado State University
Copyright 2017 Universiy of Arizona
*******************************************************************************/

#ifndef UTIL_HPP
#define UTIL_HPP

#include <sstream>
#include <stdexcept>
#include <string>
#include <iostream>
#include <vector>

template< typename Iterator >
std::string stringJoin( std::string delimiter, Iterator begin, Iterator end ){
  if( begin == end ) return std::string();

  std::ostringstream new_string;
  new_string << *begin;

  for( Iterator it = (++begin); it != end; ++it ){
    new_string << delimiter << *it;
  }
  return new_string.str();
}


/*!
Macro to create strings from ostringstreams and other <<-able objects.
Useful when trying to concatenate things together.

\note
This macro was taken from Solar's [Dev repository]
( http://rootdirectory.de/wiki/SSTR() )
It is released underCC0 1.0 Universal (CC0 1.0)
*/
#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

/*!
Exception used when we would normally use an assert.  We expect
certain conditions to apply and when they don't, we throw one of these.
*/
class assert_exception: public std::logic_error {
public:
  explicit assert_exception(const std::string& error_message) :
              std::logic_error(error_message) {}
};

/*!
Function to test a condition and throw an assert_exception if it failed.
*/
void assertWithException( bool condition, const std::string& message );

#endif
