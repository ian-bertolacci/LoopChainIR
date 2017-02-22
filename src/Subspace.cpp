/*! ****************************************************************************
\file Subspace.cpp
\authors Ian J. Bertolacci

\brief
Implements classes required for representing, managing, and iterating over Subspaces.

\copyright
Copyright 2015-2016 Colorado State University
Copyright 2017 Universiy of Arizona
*******************************************************************************/

#include "Subspace.hpp"
#include "util.hpp"
#include <iostream>
#include <sstream>

using namespace LoopChainIR;
using namespace std;


const std::string Subspace::alias_prefix = "alias_";
const std::string Subspace::const_suffix = "_c";

Subspace::Subspace( std::string var_iter_prefix, count_type var_iter_count )
: const_index( var_iter_count ), variable_iterators(),
  constant_iterator( SSTR( var_iter_prefix << Subspace::const_suffix ) ), all_iterators(),
  stage( timestamp_flags::Unstaged ),
  is_currently_aliased( false )
{
  for( count_type i = 0; i < var_iter_count; ++i ){
    this->variable_iterators.push_back( SSTR( var_iter_prefix << "_" << i) );
  }

  for( auto symbol : this->variable_iterators ){
    this->all_iterators.push_back( symbol );
  }
  this->all_iterators.push_back( this->constant_iterator);
}

Subspace::Subspace( std::string prefix, count_type count, Subspace& that )
: const_index( count ), variable_iterators(),
  constant_iterator( SSTR( prefix << "_" << that.constant_iterator ) ),
  all_iterators(), stage( timestamp_flags::Unstaged ),
  is_currently_aliased( false )
{
  assertWithException( that.variable_iterators.size() >= count, "There are fewer iterators in that subspace than needed by count." );
  for( count_type i = 0; i < count; ++i ){
    this->variable_iterators.push_back( SSTR( prefix << "_" << that.variable_iterators[i] ) );
  }

  for( auto symbol : this->variable_iterators ){
    this->all_iterators.push_back( symbol );
  }
  this->all_iterators.push_back( this->constant_iterator);
}

void Subspace::set_aliased(){
  this->is_currently_aliased = true;
}

void Subspace::unset_aliased() {
  this->is_currently_aliased = false;
}

bool Subspace::is_aliased() const {
  return this->is_currently_aliased;
}

Subspace::iterator Subspace::begin( bool use_aliases ) {
  return SubspaceIterator( this, this->all_iterators.begin(), use_aliases );
}

Subspace::iterator Subspace::begin( ) {
  return this->begin( this->is_aliased() );
}

Subspace::iterator Subspace::end() {
  return SubspaceIterator( this, this->all_iterators.end(), false );
}

Subspace::const_iterator Subspace::begin( bool use_aliases ) const {
  return ConstSubspaceIterator( this, this->all_iterators.begin(), use_aliases );
}

Subspace::const_iterator Subspace::begin( ) const {
  return this->begin( this->is_aliased() );
}

Subspace::const_iterator Subspace::end() const {
  return ConstSubspaceIterator( this, this->all_iterators.end(), false );
}

Subspace::size_type Subspace::size() const {
  return this->variable_iterators.size();
}

Subspace::size_type Subspace::complete_size() const {
  return this->all_iterators.size();
}

timestamp_t Subspace::get_stage() const {
  return this->stage;
}

std::string Subspace::get_iterators( timestamp_t stage, bool use_aliases ){
  ostringstream stream;

  if( this->get_stage() <= stage ){
    bool not_first = false;
    for( Subspace::iterator iter = this->begin( use_aliases ); iter != this->end(); ++iter ){
      stream << (not_first?",":"") << *iter ;
      not_first = true;
    }
  }

  return stream.str();
}

std::string Subspace::get( Subspace::size_type index, bool use_aliases ){
  if( this->is_aliased() && use_aliases ){
    return SSTR( alias_prefix << this->all_iterators[index] );
  } else {
    return this->all_iterators[index];
  }
}


std::string Subspace::operator[]( size_type index ){
  return this->get( index, this->is_aliased() );
}

bool Subspace::operator==( const Subspace that ) const {
  if( this->complete_size() != that.complete_size() ){
    return false;
  }

  for( Subspace::const_iterator iter_this = this->begin(false), iter_that = that.begin(false);
       iter_this != this->end() && iter_that != that.end();
       ++iter_this, ++iter_that ){
    if( *iter_this != *iter_that ){
      return false;
    }
  }

  return true;
}

bool Subspace::operator!=( const Subspace that ) const {
  return !( *this == that );
}

void Subspace::set_stage( timestamp_t stage ){
  this->stage = stage;
}

SubspaceIterator::SubspaceIterator( const Subspace* subspace, base_iterator iter, bool use_aliases )
: subspace( subspace ), internal_iter( iter ), use_aliases( use_aliases )
{ }

bool SubspaceIterator::operator==( const SubspaceIterator that ) const {
  return this->internal_iter == that.internal_iter;
}

bool SubspaceIterator::operator!=( const SubspaceIterator that ) const {
  return !( *this == that );
}

SubspaceIterator::value_type SubspaceIterator::operator*(){
  if( this->use_aliases && this->subspace->is_aliased() ){
    return SSTR( this->subspace->alias_prefix << *(this->internal_iter) );
  } else {
    return *(this->internal_iter);
  }
}

SubspaceIterator& SubspaceIterator::operator++(){
  ++(this->internal_iter);
  return *this;
}

SubspaceIterator& SubspaceIterator::operator++( int ){
  (this->internal_iter)++;
  return *this;
}

SubspaceIterator::base_iterator SubspaceIterator::get_iterator(){
  return this->internal_iter;
}


ConstSubspaceIterator::ConstSubspaceIterator( const Subspace* subspace, base_iterator iter, bool use_aliases )
: subspace( subspace ), internal_iter( iter ), use_aliases( use_aliases )
{ }

bool ConstSubspaceIterator::operator==( const ConstSubspaceIterator that ) const {
  return this->internal_iter == that.internal_iter;
}

bool ConstSubspaceIterator::operator!=( const ConstSubspaceIterator that ) const {
  return !( *this == that );
}

ConstSubspaceIterator::value_type ConstSubspaceIterator::operator*(){
  if( this->use_aliases && this->subspace->is_aliased() ){
    return SSTR( this->subspace->alias_prefix << *(this->internal_iter) );
  } else {
    return *(this->internal_iter);
  }
}

ConstSubspaceIterator& ConstSubspaceIterator::operator++(){
  ++(this->internal_iter);
  return *this;
}

ConstSubspaceIterator& ConstSubspaceIterator::operator++( int ){
  (this->internal_iter)++;
  return *this;
}

ConstSubspaceIterator::base_iterator ConstSubspaceIterator::get_iterator(){
  return this->internal_iter;
}

SubspaceManager::SubspaceManager( Subspace* loop, Subspace* nest )
: subspaces(), loop( loop ), nest( nest ), stage( timestamp_flags::BaseStage ),
  safe_prefixes()
{
  this->loop->set_stage( this->get_current_stage() );
  this->subspaces.push_back( this->loop );
  this->nest->set_stage( this->get_current_stage() );
  this->subspaces.push_back( this->nest );
}

SubspaceManager::iterator SubspaceManager::begin(){
  return this->subspaces.begin();
}

SubspaceManager::iterator SubspaceManager::end(){
  return this->subspaces.end();
}

SubspaceManager::iterator SubspaceManager::get_iterator_to_subspace( Subspace* subspace ){
  SubspaceManager::iterator iter = this->subspaces.begin();
  while( iter != this->subspaces.end() && *iter != subspace ){
    ++iter;
  }
  return iter;
}

SubspaceManager::iterator SubspaceManager::get_iterator_to_loops(){
  return this->get_iterator_to_subspace( this->loop );
}

SubspaceManager::iterator SubspaceManager::get_iterator_to_nest(){
  return this->get_iterator_to_subspace( this->nest );
}

Subspace* SubspaceManager::get_loops(){
  return *( this->get_iterator_to_loops() );
}

Subspace* SubspaceManager::get_nest(){
  return *( this->get_iterator_to_nest() );
}

std::string SubspaceManager::get_safe_prefix( std::string base ){
  if( this->safe_prefixes.count( base ) == 0 ){
    this->safe_prefixes[base] = 1;
  }

  std::ostringstream prefix;
  for( int i = 0; i < this->safe_prefixes[base]; ++i ){
    prefix << base;
  }

  this->safe_prefixes[base] += 1;

  return prefix.str();
}

SubspaceManager::iterator SubspaceManager::insert_left( Subspace* subspace, SubspaceManager::iterator cursor ){
  subspace->set_stage( this->get_output_stage() );
  return this->subspaces.insert( cursor, subspace );
}

SubspaceManager::iterator SubspaceManager::insert_right( Subspace* subspace, SubspaceManager::iterator cursor ){
  return this->insert_left( subspace, std::next(cursor) );
}

Subspace::size_type SubspaceManager::size(){
  Subspace::size_type size = 0;
  for( Subspace* subspace : *this ){
    size += subspace->complete_size();
  }
  return size;
}

timestamp_t SubspaceManager::get_current_stage(){
  return this->stage;
}

timestamp_t SubspaceManager::get_input_stage(){
  return this->get_current_stage();
}

timestamp_t SubspaceManager::get_output_stage() {
  return this->get_input_stage() + 1;
}

timestamp_t SubspaceManager::advance_stage(){
  this->stage += 1;
  return this->get_current_stage();
}

timestamp_t SubspaceManager::next_stage(){
  for( Subspace* subspace : *this ){
    subspace->unset_aliased();
  }
  return this->advance_stage();
}

std::string SubspaceManager::get_iterators( timestamp_t stage, bool use_aliases ){
  ostringstream stream;

  bool not_first = false;
  for( Subspace* subspace : *this ){
    std::string subspace_str =  subspace->get_iterators( stage, use_aliases );

    if( subspace_str != "" ){
      stream << (not_first? "," : "") << subspace_str;
    }

    not_first = not_first || (subspace_str != "");
  }

  return stream.str();
}

std::string SubspaceManager::get_iterators( ){
  return this->get_iterators( this->get_current_stage(), true );
}

std::string SubspaceManager::get_input_iterators(){
  return this->get_iterators( this->get_input_stage(), false );
}

std::string SubspaceManager::get_output_iterators(){
  return this->get_iterators( this->get_output_stage(), true );
}
