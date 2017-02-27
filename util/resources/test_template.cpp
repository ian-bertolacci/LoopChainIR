#include <vector>
#include <tuple>
#include <map>
#include <set>
#include <string>
#include <iostream>
using namespace std;

#define max(x,y)  (((x)>(y))?(x):(y))
#define min(x,y)  (((x)<(y))?(x):(y))

#if ASSUME_POSITIVE_INTMOD
#define intDiv(x,y)	(eassert(((x)%(y)) >= 0), ((x)/(y)))
#define intMod(x,y)	(eassert(((x)%(y)) >= 0), ((x)%(y)))
#else
#define intDiv_(x,y)  ((((x)%(y))>=0) ? ((x)/(y)) : (((x)/(y)) -1))
#define intMod_(x,y)  ((((x)%(y))>=0) ? ((x)%(y)) : (((x)%(y)) +y))
#define checkIntDiv(x,y) (eassert((y) > 0 && intMod_((x),(y)) >= 0 && intMod_((x),(y)) <= (y) && x==((y)*intDiv_((x),(y)) + intMod_((x),(y)))))
#define intDiv(x,y)	(checkIntDiv((x),(y)), intDiv_((x),(y)))
#define intMod(x,y)	(checkIntDiv((x),(y)), intMod_((x),(y)))
#endif

#define floord(n, d)  intDiv((n), (d))

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
    return isIn( node ) && this->marked[ node ];
  }

  bool isSatisfied( Tuple_Type node ){
    set< Tuple_Type > deps = graph[ node ];
    bool satisfied = true;

    for( auto it = deps.begin(); it != deps.end(); ++it ){
      Tuple_Type iteration = *it;
      // Need to check marking.
      // A dependant iteration may be satisfied on it's end, but is MUST be
      // marked for the dependency to be satisfied.
      // Satisfaction is not tested because it would be recursive.
      satisfied &= isMarked( iteration );

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
  Graph< tuple< TUPLE_TYPE_STAMP > > dependency_graph;
  Graph< tuple< TUPLE_TYPE_STAMP > > new_order_graph;
  {
/*****************************************************************************/
/********** BEGIN INJECTED CODE FROM DEPENDENCIES GENERATORS *****************/
COMPARISON_CODE_STAMP
/********** END INJECTED CODE FROM DEPENDENCIES GENERATORS *******************/
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
