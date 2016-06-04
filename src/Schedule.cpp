/*! ****************************************************************************
\file Schedule.cpp
\autors Ian J. Bertolacci

\brief
Generate a simple sequential, non-parallel schedule from a LoopChain.

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/

#include "Schedule.hpp"
#include "all_isl.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <unistd.h>

using namespace LoopChainIR;

Schedule::Schedule( LoopChain& chain, std::string statement_prefix ) :
  chain(chain), statement_prefix(statement_prefix),
  root_statement_symbol( SSTR(statement_prefix << "statement_" ) )
  {

  // String of domains mapping to chain
  std::ostringstream ordering_string;
  ordering_string << "{";

  // The maximum dimensionality of all the domains in the chain.
  // this is used to pad iterations that are of lower dimensionality.
  RectangularDomain::size_type max_dims = this->chain.maxDimension();

  for( LoopChain::size_type nest_idx = 0; nest_idx < this->chain.length(); nest_idx += 1 ){
    LoopNest& nest = this->chain.getNest( nest_idx );
    RectangularDomain& domain = nest.getDomain();

    // String of statement
    std::ostringstream statement_string;

    // String of padding for statement;
    std::ostringstream padding_string;

    // String of symbolic bounds
    std::ostringstream symbolic_string;

    bool is_not_first_symbolic = false; // for comma insertion
    for( RectangularDomain::size_type symbolic = 0; symbolic < domain.symbolics(); symbolic += 1 ){
      if( is_not_first_symbolic ){
        symbolic_string << ",";
      }
      symbolic_string << domain.getSymbol(symbolic);
      is_not_first_symbolic = true;
    }

    // String of bounds inequalities
    std::ostringstream inequalities_string;

    for( RectangularDomain::size_type dimension = 0; dimension < domain.dimensions(); dimension += 1 ){
      std::string lower = domain.getLowerBound( dimension );
      std::string upper = domain.getUpperBound( dimension );

      std::string index_symbol = SSTR( "idx_" << dimension );

      // add index to statement string
      if( dimension > 0 ) statement_string << ",";
      statement_string << index_symbol;

      // add to inequalities string
      if( dimension > 0 ) inequalities_string << " and ";
      inequalities_string << lower << " <= " << index_symbol << " <= " << upper;

    }// for_each dimension

    // Expand the iterator string if necessary
    for( RectangularDomain::size_type dimension = domain.dimensions(); dimension < max_dims; dimension += 1 ){
      padding_string << ",0";
    }

    // Create the full string representing an ISL Domain
    std::string domain_string = SSTR( "[" << symbolic_string.str() << "] -> {"
                                          << root_statement_symbol << nest_idx << "[" << statement_string.str() << "] : "
                                          << inequalities_string.str() << "}"
                                    );

    // Construct actual ISL domain and append it.
    domains.push_back( domain_string.c_str() );

    // Create expansion transformation string (i.e. { [i,j] -> [$k,i,j,0] })
    // where $k is the loop's position in the chain.
    std::string expanded_form = SSTR( "[" << nest_idx << "," << statement_string.str() << ",0" << padding_string.str() << "]" );

    // Create the loop chain map string
    ordering_string << root_statement_symbol << nest_idx << "[" << statement_string.str() << "]"
               << " -> " << expanded_form << "; ";

  }// for_each nest

  ordering_string << "}";

  this->append( ordering_string.str() );

  this->iterators_length = max_dims + 2;

}

void Schedule::apply( Transformation& scheduler ){
  this->append( scheduler.apply(*this) );
}

void Schedule::apply( std::vector<Transformation*> schedulers ){
  for( std::vector<Transformation*>::iterator it = schedulers.begin(); it != schedulers.end(); ++it ){
    this->apply( **it );
  }
}

Schedule::size_type Schedule::append( std::string text ){
  if( text != "" ){
    this->transformations.push_back( text );
  }

  return this->transformations.size()-1;
}

Schedule::iterator Schedule::begin_domains(){
  return this->domains.begin();
}

Schedule::iterator Schedule::end_domains(){
  return this->domains.end();
}

Schedule::iterator Schedule::begin_transformations(){
  return this->transformations.begin();
}

Schedule::iterator Schedule::end_transformations(){
  return this->transformations.end();
}


Schedule::const_iterator Schedule::begin_domains() const {
  return this->domains.begin();
}

Schedule::const_iterator Schedule::end_domains() const {
  return this->domains.end();
}

Schedule::const_iterator Schedule::begin_transformations() const {
  return this->transformations.begin();
}

Schedule::const_iterator Schedule::end_transformations() const {
  return this->transformations.end();
}


std::string Schedule::codegen( ){
  isl_ctx* ctx = isl_ctx_alloc();

  // Union domains together
  isl_union_set* full_domain = NULL;

  for( Schedule::iterator it = this->begin_domains(); it != this->end_domains(); ++it ){
    isl_union_set* domain = isl_union_set_read_from_str(ctx, (*it).c_str());
    if( full_domain == NULL ){
      full_domain = domain;
    } else {
      full_domain = isl_union_set_union( full_domain, domain );
    }
  }

  // Compose tranformation maps together
  isl_union_map* transformation = NULL;

  for( Schedule::iterator it = this->begin_transformations(); it != this->end_transformations(); ++it){
    isl_union_map* map = isl_union_map_read_from_str(ctx, (*it).c_str());
    transformation = (transformation)? isl_union_map_apply_range(transformation, map) : map;
  }

  // Apply transformation to schedule
  isl_union_map* schedule = isl_union_map_intersect_domain(transformation, full_domain);

  // Create a memory file (and stream) to write to
  int bytes = 1028*4;
  int type_size = sizeof(char);

  FILE* memory_file = fmemopen( calloc(bytes, type_size), bytes*type_size, "r+" );

  // Create AST and write to memory_file
  isl_ast_build* build = isl_ast_build_alloc(ctx);
  isl_ast_node* tree = isl_ast_build_node_from_schedule_map(build, schedule);
  isl_printer* p = isl_printer_to_file(ctx, memory_file);
               p = isl_printer_set_output_format(p, ISL_FORMAT_C);
               p = isl_printer_print_ast_node(p, tree);

  // Read text and close file
  std::string code_text;
  std::fseek( memory_file, 0, SEEK_END );
  code_text.resize( std::ftell(memory_file) );
  std::rewind(memory_file);
  std::fread(&code_text[0], 1, code_text.size(), memory_file );
  std::fclose( memory_file );

  // Fix weird issue where isl generates empty code body (i.e. {\n}\n)
  // but leaves in the remaining file as null bytes.
  // somehow this gets copied over from the FILE to the string and makes a big
  // mess of things. Here we find the first null character and trim to it.
  std::size_t find_null = code_text.find( '\0' );
  if( find_null != std::string::npos ){
    code_text.resize( find_null );
  }

  // Free ISL objects
  /*
  TODO Figure out how to free these objects...
  isl_union_map_free( chain_map ); // breaks.
  isl_union_map_free( schedule ); // breaks
  isl_union_set_free( full_domain ); // breaks
  breaks
  for( std::vector<isl_union_set*>::iterator it = domains.begin(); it != domains.end(); ++it ){
    isl_union_set_free(*it);
  }
  */

  isl_ast_build_free( build );
  isl_printer_free( p );
  isl_ast_node_free( tree );
  isl_ctx_free( ctx );

  return code_text;
}

bool Schedule::codegenToFile( std::string file_name ){
  std::string code = this->codegen();
  std::ofstream file_stream( file_name );

  if( file_stream ){
    file_stream << code;
    file_stream.flush();
  }

  return file_stream.good() && !( file_stream.fail() || file_stream.bad() );
}

RectangularDomain::size_type Schedule::getIteratorsLength() {
  return this->iterators_length;
}

RectangularDomain::size_type Schedule::modifyIteratorsLength( int delta ){
  if( (RectangularDomain::size_type) abs( delta ) > this->getIteratorsLength() ){
    // EXCEPTION?
  }

  this->iterators_length += delta;

  return this->getIteratorsLength();
}

std::string Schedule::getStatementPrefix(){
  return std::string(this->statement_prefix);
}

std::string Schedule::getRootStatementSymbol(){
  return std::string(this->root_statement_symbol);
}

std::ostream& LoopChainIR::operator<<( std::ostream& os, const Schedule& schedule){
  os << "Domains:" << std::endl;

  for( Schedule::const_iterator it = schedule.begin_domains(); it != schedule.end_domains(); ++it ){
    os << (*it) << std::endl;
  }

  os << std::endl << "Transformations:" << std::endl;
  for( Schedule::const_iterator it = schedule.begin_transformations(); it != schedule.end_transformations(); ++it ){
    os << (*it) << std::endl;
  }

  return os;
}
