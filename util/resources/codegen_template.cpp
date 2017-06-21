#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include <LoopChainIR/all_isl.hpp>
#include <LoopChainIR/LoopChain.hpp>
#include <LoopChainIR/LoopNest.hpp>
#include <LoopChainIR/RectangularDomain.hpp>
#include <LoopChainIR/DefaultSequentialTransformation.hpp>
#include <LoopChainIR/ShiftTransformation.hpp>
#include <LoopChainIR/FusionTransformation.hpp>
#include <LoopChainIR/TileTransformation.hpp>
#include <LoopChainIR/AutomaticShiftTransformation.hpp>
#include <LoopChainIR/WavefrontTransformation.hpp>
#include <LoopChainIR/Schedule.hpp>
#include <LoopChainIR/Transformation.hpp>

using namespace std;
using namespace LoopChainIR;

void simpleGenerateFromString( string expression, FILE* file ){
  isl_ctx* ctx;
  isl_ast_build* build;
  isl_ast_node* tree;
  isl_printer* p;

  ctx = isl_ctx_alloc();

  isl_union_set* domain = isl_union_set_read_from_str( ctx, expression.c_str() );

  isl_schedule* schedule = isl_schedule_from_domain( domain );

  build = isl_ast_build_alloc( ctx );

  // Code generation.
  tree = isl_ast_build_node_from_schedule(build, schedule);

  isl_ast_build_free(build);

  p = isl_printer_to_file( ctx, file );

  p = isl_printer_set_output_format(p, ISL_FORMAT_C);
  p = isl_printer_print_ast_node(p, tree);
  isl_printer_free(p);

  isl_ast_node_free(tree);

  isl_ctx_free(ctx);
}

void generateGraphCode(){
  vector<string> expressions;
/*****************************************************************************/
/********** BEGIN INJECTED CODE FOR CODEGEN **********************************/
// GENERATED_GRAPH_CODE_LIST_STAMP
/********** END OF INJECTED CODE FOR CODEGEN *********************************/
/*****************************************************************************/
/*****************************************************************************/

  FILE* file = fopen( "GRAPH_CODEGEN_OUTPUT_NAME_STAMP", "w");

  for( vector<string>::iterator it = expressions.begin(); it != expressions.end(); ++it ){
    string expression = *it;
    fprintf( file, "// Generated from:\n// %s\n", expression.c_str() );
    simpleGenerateFromString( *it, file );
    fprintf( file, "\n" );
  }
}

void writeTextToFile( string text, string filename ){
  ofstream output;
  output.open( filename );
  output << text ;
  output.close();
}

void generateChainCode(){
/*****************************************************************************/
/********** BEGIN INJECTED CODE FOR CODEGEN **********************************/
// GENERATED_GENERATOR_CODE_STAMP
/********** END OF INJECTED CODE FOR CODEGEN *********************************/
/*****************************************************************************/
/*****************************************************************************/
}


int main(){
/*****************************************************************************/
/********** BEGIN INJECTED CODE FOR CODEGEN **********************************/
GENERATED_MAIN_CODE_STAMP
/********** END OF INJECTED CODE FOR CODEGEN *********************************/
/*****************************************************************************/
/*****************************************************************************/
}
