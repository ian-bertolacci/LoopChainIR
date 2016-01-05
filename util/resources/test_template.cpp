#include <vector>
#include <tuple>
#include <map>
#include <set>
#include <string>
#include <iostream>
using namespace std;

template< class Tuple_Type >
class Graph {
private:
  map< Tuple_Type, set<Tuple_Type> > graph;
  map< Tuple_Type, bool > marked;

public:
  Graph(){ }

  bool isIn( Tuple_Type node ){
    return graph.count( node ) > 0;
  }

  void insert_node( Tuple_Type node ){
    graph.insert( pair<Tuple_Type, set<Tuple_Type> >( node, set<Tuple_Type>() ) );
    marked.insert( pair<Tuple_Type, bool>( node, false ) );
  }

  void connect( Tuple_Type first, Tuple_Type second ){
    if( !isIn( first ) ){
      insert_node( first );
    }

    if( !isIn( second ) ){
      insert_node( second );
    }

    graph[ second ].insert( first );
  }

  void mark( Tuple_Type node ){
    this->marked[ node ] = true;
  }

  bool isMarked( Tuple_Type node ){
    return isIn( node ) && this->marked( node );
  }

  bool isSatisfied( Tuple_Type node ){
    set< Tuple_Type > deps = graph[ node ];
    bool satisfied = true;

    for( auto it = deps.begin(); it != deps.end(); ++it ){
      satisfied &= marked[ *it ];

      if( !satisfied ) break;
    }
    return satisfied;
  }

  set< Tuple_Type > getDependencies( Tuple_Type node ){
    return this->graph[node];
  }

};


/*****************************************************************************/
/********** BEGIN INJECTED CODE FROM BOUNDS CHOOSER **************************/
BOUNDS_CODE_STAMP
/********** END OF INJECTED CODE FROM BOUNDS CHOOSER *************************/
/*****************************************************************************/
/*****************************************************************************/

int main( ){
  Graph< tuple< TUPLE_TYPE_STAMP > > graph;
  {
/*****************************************************************************/
/********** BEGIN INJECTED CODE FROM HUMAN COMPARISON ************************/
COMPARISON_CODE_STAMP
/********** END OF INJECTED CODE FROM HUMAN COMPARISON ***********************/
/*****************************************************************************/
  }

  int code = 0;
  {
/*****************************************************************************/
/********** BEGIN INJECTED CODE FROM CODE GENERATOR **************************/
GENERATED_CODE_STAMP
/********** END OF INJECTED CODE FROM CODE GENERATOR *************************/
/*****************************************************************************/
  }

  int exit_code;

  switch( code ){
    case 0:{
      // Test successful
      exit_code = 0;
      break;
    }
    case -1:{
      // Test unsuccessful
      exit_code = -1;
      break;
    }
    default:{
      // Undefined exit code;
      exit_code = -9999;
      break;
    }
  }

  return exit_code;
}
