/*******************************************************************************
\file DefaultSequentialSchedule.cpp
\autors Ian J. Bertolacci

\purpose
Generate a simple sequential, non-parallel schedule from a LoopChain.

\copyright
Copyright 2015 Colorado State University
*******************************************************************************/

#include "DefaultSequentialSchedule.h"

DefaultSequentialSchedule::DefaultSequentialSchedule( LoopChain& chain){
  this->chain = chain;
}

LoopChain& DefaultSequentialSchedule::getChain(){
  return this->chain;
}

void DefaultSequentialSchedule::codegen( FILE* output_file ){
  isl_ctx* ctx = isl_ctx_alloc();

  // create string domains
  std::vector<isl_union_set*> domains;

  // String of domains mapping to chain
  std::ostringstream map_string;
  map_string << "{";

  for( LoopChain::size_type nest_idx = 0; nest_idx < this->chain.length(); nest_idx += 1 ){
    LoopNest& nest = this->chain.getNest( nest_idx );
    Box& domain = nest.getDomain();

    // String of statement
    std::ostringstream statement_string;
    //statement_string << ;

    // String of symbolic bounds
    std::ostringstream symbolic_string;

    // String of bounds inequalities
    std::ostringstream inequalities_string;

    bool is_not_first_symbolic = false; // for comma insertion
    for( Box::size_type dimension = 0; dimension < domain.dimensions(); dimension += 1 ){
      std::string lower = domain.getLowerBound( dimension );
      std::string upper = domain.getUpperBound( dimension );

      std::string index_symbol = SSTR( "idx_" << dimension );

      // add index to statement string
      if( dimension > 0 ) statement_string << ",";
      statement_string << index_symbol;

      // add to inequalities string
      if( dimension > 0 ) inequalities_string << " and ";
      inequalities_string << lower << " <= " << index_symbol << " <= " << upper;


      // find and add symolic bounds to symbolic string
      if( lower.find_first_of(ALPHA_CHARS) != std::string::npos ){
        if( is_not_first_symbolic ){
          symbolic_string << ",";
        }
        symbolic_string << lower;
        is_not_first_symbolic = true;
      }

      if( upper.find_first_of(ALPHA_CHARS) != std::string::npos ){
        if( is_not_first_symbolic ){
          symbolic_string << ",";
        }
        symbolic_string << upper;
        is_not_first_symbolic = true;
      }
    }// for_each dimension

    // Create the full string representing an ISL Domain
    std::string domain_string = SSTR( "[" << symbolic_string.str() << "] -> {"
                                          << "statement_" << nest_idx << "[" << statement_string.str() << "] : "
                                          << inequalities_string.str() << "}"
                                    );

    domains.push_back( isl_union_set_read_from_str(ctx, domain_string.c_str() ) );
    //std::cout << domain_string << std::endl;

    // Create the loop chain map string
    map_string << "statement_" << nest_idx << "[" << statement_string.str() << "]"
               << " -> [" << nest_idx << "," << statement_string.str() << "]; ";

  }// for_each nest

  map_string << "}";

  // Union the domains together
  isl_union_set* full_domain = NULL;

  for( std::vector<isl_union_set*>::iterator it = domains.begin(); it != domains.end(); ++it ){
    if( full_domain == NULL ){
      full_domain = *it;
    } else {
      full_domain = isl_union_set_union( full_domain, *it );
    }
  }

  /
  isl_union_map* chain_map = isl_union_map_read_from_str(ctx, map_string.str().c_str() );
  isl_union_map* schedule = isl_union_map_intersect_domain(chain_map, full_domain);
  isl_ast_build* build = isl_ast_build_alloc(ctx);
  isl_ast_node* tree = isl_ast_build_node_from_schedule_map(build, schedule);
  isl_printer* p = isl_printer_to_file(ctx, output_file);
               p = isl_printer_set_output_format(p, ISL_FORMAT_C);
               p = isl_printer_print_ast_node(p, tree);

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

}
