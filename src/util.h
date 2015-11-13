/*******************************************************************************
\file util.h
\autors Ian J. Bertolacci

\purpose
Contains a number of utility fuctions.

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/

#include <sstream>


/*
\note
This macro was taken from Solar's [Dev repository]
( http://rootdirectory.de/wiki/SSTR() )
It is released underCC0 1.0 Universal (CC0 1.0)
*/
#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()
