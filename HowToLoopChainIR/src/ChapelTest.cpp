#include <LoopChainIR/LoopChain.hpp>
#include <LoopChainIR/LoopNest.hpp>
#include <LoopChainIR/Accesses.hpp>

#include <LoopChainIR/DefaultSequentialTransformation.hpp>
#include <LoopChainIR/ShiftTransformation.hpp>
#include <LoopChainIR/FusionTransformation.hpp>
#include <LoopChainIR/AutomaticShiftTransformation.hpp>
#include <LoopChainIR/TileTransformation.hpp>
#include <LoopChainIR/WavefrontTransformation.hpp>

#include <LoopChainIR/ChapelTransformationWalker.hpp>

#include <LoopChainIR/util.hpp>

#include <iostream>
#include <fstream>
#include <utility>
#include <list>

using namespace std;
using namespace LoopChainIR;

int main( int argc, char* argv[] ){

  LoopChain chain;

  chain.append(
    /*
      Create a single loop-nest encoding
      for( int i = 1; i <= N; ++i )
        B[i] = A[i-1] + A[i] + A[i+1];
    */
    LoopNest(
      // Create the nest's domain
      RectangularDomain(
        // Bounds as pairs [lower, upper].
        { make_pair( "lb1i", "ub1i" ), make_pair( "lb1j", "ub1j" ) },
        // Set of symbolic constants found in any of the bounds.
        {"lb1i","ub1i","lb1j","ub1j"}
      ),
      // List of the nest's read and write accesses.
      // Order is unimportant.
      {
        // Encode accesses of dataspace 'A' (a.k.a. the array 'A').
        Dataspace(
          // Name
          "A",
          // Reads A[i-1], A[i], A[i+1]
          TupleCollection({ Tuple({-1,0}), Tuple({0,0}), Tuple({1,0}), Tuple({0,-1}),Tuple({0,1}) }),
          // Writes nothing. Create an empty collection of 1-D tuples
          TupleCollection( 2 )
        ),
        // Encode accesses of dataspace 'B' (a.k.a. the array 'B').
        Dataspace(
          // Name
          "B",
          // Reads nothing. Create an empty collection of 1-D tuples
          TupleCollection( 2 ),
          // Writes B[i]
          TupleCollection({ Tuple({0,0}) })
        )
      }
    )
  );

  chain.append(
    /*
      Create a single loop-nest encoding
      for( int i = 1; i <= N; ++i )
        A[i] = B[i-1] + B[i] + B[i+1];
    */
    LoopNest(
      // Create the nest's domain
      RectangularDomain(
        // Bounds as pairs [lower, upper].
        { make_pair( "lb2i", "ub2i" ), make_pair( "lb2j", "ub2j" ) },
        // Set of symbolic constants found in any of the bounds.
        {"lb2i","ub2i","lb2j","ub2j"}
      ),
      // List of the nest's read and write accesses.
      // Order is unimportant.
      {
        // Encode accesses of dataspace 'A' (a.k.a. the array 'A').
        Dataspace(
          // Name
          "A",
          // Reads nothing. Create an empty collection of 1-D tuples
          TupleCollection( 2 ),
          // Writes C[i].
          TupleCollection({ Tuple({0,0}) })
        ),
        // Encode accesses of dataspace 'B' (a.k.a. the array 'B').
        Dataspace(
          // Name
          "B",
          // Reads B[i-1], B[i], B[i+1]
          TupleCollection({ Tuple({-1,0}), Tuple({0,0}), Tuple({1,0}), Tuple({0,-1}),Tuple({0,1}) }),
          // nothing. Create an empty collection of 1-D tuples
          TupleCollection( 2 )
        )
      }
    )
  );

  list< tuple<string,string,vector<Transformation*> > > test_transformations = {
    make_tuple(
      "Default transformation",
      "default",
      (vector<Transformation*>) {
        // First create shift transformation for the second loop-nest (loop 1)
        new DefaultSequentialTransformation( )
      }
    ),
    make_tuple(
      "automatic-shift, fuse",
      "s.f",
      (vector<Transformation*>) {
        new AutomaticShiftTransformation(),
        // Next create fusion transformation
        new FusionTransformation(
          // Ordered list of loop indexes to fuse.
          (vector<LoopChain::size_type>){ 0, 1 }
        )
      }
    ),
    make_tuple(
      "automatic-shift, fuse, tile (8,8)",
      "s.f.t_8_8",
      (vector<Transformation*>) {
        new AutomaticShiftTransformation(),
        new FusionTransformation(
          (vector<LoopChain::size_type>){ 0, 1 }
        ),
        new TileTransformation(
          0,
          TileTransformation::TileMap( { { 0, "8" }, { 1, "8" } } )
        )
      }
    ),
    make_tuple(
      "automatic-shift, tile (8,8), fuse",
      "s.t_8_8.f",
      (vector<Transformation*>) {
        new AutomaticShiftTransformation(),
        new TileTransformation(
          0,
          TileTransformation::TileMap( { { 0, "8" }, { 1, "8" } } )
        ),
        new FusionTransformation(
          (vector<LoopChain::size_type>){ 0, 1 }
        )
      }
    ),
    make_tuple(
      "automatic-shift, fuse, tile (8,8), wavefront",
      "s.f.t_8_8.w",
      (vector<Transformation*>) {
        new AutomaticShiftTransformation(),
        new FusionTransformation(
          (vector<LoopChain::size_type>){ 0, 1 }
        ),
        new TileTransformation(
          0,
          TileTransformation::TileMap( { { 0, "8" }, { 1, "8" } } )
        ),
        new WavefrontTransformation()
      }
    ),
    make_tuple(
      "automatic-shift, tile (8,8), fuse, wavefront",
      "s.t_8_8.f.w",
      (vector<Transformation*>) {
        new AutomaticShiftTransformation(),
        new TileTransformation(
          0,
          TileTransformation::TileMap( { { 0, "8" }, { 1, "8" } } )
        ),
        new FusionTransformation(
          (vector<LoopChain::size_type>){ 0, 1 }
        ),
        new WavefrontTransformation()
      }
    )
  };

  string chapel_preamble =
    "config const lb1i = 1;\n"
    "config const ub1i = 32;\n"
    "config const lb1j = 1;\n"
    "config const ub1j = 32;\n"
    "config const lb2i = 1;\n"
    "config const ub2i = 32;\n"
    "config const lb2j = 1;\n"
    "config const ub2j = 32;\n"
    "proc statement( name, i,j ){\n"
    "  writeln( name, (i,j) );\n"
    "}\n"
    "proc statement_0( i, j ){\n"
    "  statement( \"s_0\", i, j );\n"
    "}\n"
    "proc statement_1( i, j ){\n"
    "  statement( \"s_1\", i, j );\n"
    "}\n";

  string cpp_preamble =
   "#include<iostream>\n"
   "#define floord(x, y) ( ((((x)%(y))>=0) ? ((x)/(y)) : (((x)/(y))) -1 ) )\n"
   "using namespace std;\n"
   "const int lb1i = 1;\n"
   "const int ub1i = 32;\n"
   "const int lb1j = 1;\n"
   "const int ub1j = 32;\n"
   "const int lb2i = 1;\n"
   "const int ub2i = 32;\n"
   "const int lb2j = 1;\n"
   "const int ub2j = 32;\n"
   "void statement( string name, int i, int j ){\n"
   "  cout << name << \"(\" << i << \", \" << j << \")\" << endl;\n"
   "}\n"
   "void statement_0( int i, int j ){\n"
   "  statement( \"s_0\", i, j );\n"
   "}\n"
   "void statement_1( int i, int j ){\n"
   "  statement( \"s_1\", i, j );\n"
   "}\n";


  for( tuple<string, string, vector<Transformation*> > test : test_transformations )
  {
    string transformation_description = get<0>( test );
    string output_filename = get<1>( test );
    vector<Transformation*> transformations = get<2>( test );

    // Create a schedule from the loop-chain
    Schedule schedule( chain );

    // Apply transformations to schedule
    schedule.apply( transformations );

    // Print state initial state.
    // Schedule can generate code immediately

    cout  << "========================================================" << endl
          << "After " << transformation_description << " transfomations" << endl
          << "--------------------------------------------------------" << endl
          << "ISCC code" << endl
          << "--------------------------------------------------------" << endl
          << schedule.codegenToISCC() << endl
          << "--------------------------------------------------------" << endl
          << "C Code" << endl
          << "--------------------------------------------------------" << endl
          << schedule.codegen() << endl
          << "--------------------------------------------------------" << endl
          << "Chapel Code" << endl
          << "--------------------------------------------------------" << endl
          << convertISLtoChapel( schedule.codegenToIslAst() ) << endl
          << "========================================================" << endl;

    {
      ofstream output;
      output.open( SSTR( output_filename << ".chpl" ) );
      output << chapel_preamble << endl;
      output << convertISLtoChapel( schedule.codegenToIslAst() ) << endl;
      output.close();
    }

    {
      ofstream output;
      output.open( SSTR( output_filename << ".cpp" ) );
      output << cpp_preamble << "int main(){\n" << schedule.codegen() << "\n}" << endl;
      output.close();
    }

  }


  return 0;
}
