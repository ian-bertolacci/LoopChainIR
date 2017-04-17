#include "AutomaticShiftTransformation.hpp"
#include <linear_solver/linear_solver.h>
#include <linear_solver/linear_solver.pb.h>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <list>
#include <limits>

using namespace std;
using namespace LoopChainIR;
using namespace operations_research;

AutomaticShiftTransformation::AutomaticShiftTransformation(){ }

std::vector<std::string> AutomaticShiftTransformation::apply( Schedule& schedule ){
  return this->apply( schedule, *(std::next(schedule.getSubspaceManager().get_iterator_to_loops())));
}

std::vector<std::string> AutomaticShiftTransformation::apply( Schedule& schedule, Subspace* subspace ){
  std::vector<std::string> transformations;

  std::vector<ShiftTransformation*> shift_transformations = this->computeShiftForFusion( subspace->size() , schedule.getChain() );
  for( ShiftTransformation* shift : shift_transformations ){
    std::vector<std::string> shifts = shift->apply( schedule );
    transformations.insert( transformations.end(), shifts.begin(), shifts.end() );
  }

  return transformations;
}


std::vector<ShiftTransformation*> AutomaticShiftTransformation::computeShiftForFusion( Subspace::size_type dimensions, LoopChain chain ){
  MPSolver solver("ShiftSolver", MPSolver::CBC_MIXED_INTEGER_PROGRAMMING);

  const double infinity = solver.infinity();

  std::map< LoopChain::size_type, std::vector<MPVariable*>* > shift_variables;

  MPObjective* objective = solver.MutableObjective();
  std::vector< MPConstraint* > constraints;

  int max_on_dims[dimensions];
  for( int& initial_max : max_on_dims ){
    initial_max = numeric_limits<int>::min();
  }

  std::map< LoopChain::size_type, std::map<std::string, Dataspace> > dataspaces;
  std::map< LoopChain::size_type, std::set<std::string> > dataspace_names;

  for( LoopChain::size_type nest_idx = 0; nest_idx < chain.length(); ++nest_idx ){
    // Make shift tuple for this nest
    std::vector<MPVariable*>* nest_shifts = new std::vector<MPVariable*>();
    solver.MakeIntVarArray( dimensions,
                            0, infinity,
                            SSTR("shift_" << nest_idx << "_"),
                            nest_shifts
                          );
    shift_variables[nest_idx] = nest_shifts;

    // Setup objective SUM( S_yd - S_xd )
    for( MPVariable* variable : *nest_shifts ){
      //objective->SetCoefficient( variable, 2*((double)nest_idx) - ((double)chain.length())+1 );
      objective->SetCoefficient( variable, 1 );
    }

    std::list<Dataspace> nest_unnamed_dataspaces = chain.getNest(nest_idx).getDataspaces();
    std::map< std::string, Dataspace > nest_dataspaces;
    std::set<std::string> nest_dataspace_names;

    for( Dataspace dataspace : nest_unnamed_dataspaces ){
      nest_dataspace_names.insert( dataspace.name );
      nest_dataspaces.emplace( dataspace.name, dataspace );
    }

    dataspaces[nest_idx] = nest_dataspaces;
    dataspace_names[nest_idx] = nest_dataspace_names;

    // Doing things with respect to the previous nests
    for( LoopChain::size_type previous_idx = 0; previous_idx < nest_idx; ++previous_idx ){
      std::vector<MPVariable*>* previous_shifts = shift_variables[previous_idx];

      // Setup constraints: c_ywd - C_xwd <= S_yd - S_xd
      std::map<std::string, Dataspace> previous_dataspaces = dataspaces[previous_idx];

      std::set<std::string> previous_dataspace_names;
      for( std::map<std::string, Dataspace>::iterator iter = previous_dataspaces.begin(); iter != previous_dataspaces.end(); ++iter ){
        previous_dataspace_names.insert( iter->first );
      }

      // Determine what dataspaces the pair of loops share
      std::vector<std::string> common( previous_dataspace_names.size() + nest_dataspace_names.size(), "" );

      std::vector<std::string>::iterator last_insert = std::set_intersection(
                            previous_dataspace_names.begin(), previous_dataspace_names.end(),
                            nest_dataspace_names.begin(), nest_dataspace_names.end(),
                            common.begin()
                          );

      common.resize( last_insert - common.begin() );

      // pair of loops do not share any dataspaces
      if( common.size() < 1 ){
        continue;
      }
      cout << "=========================================\n"
           << nest_idx << " vs " << previous_idx << endl;
      for( std::string name : common ){

        auto calc_func = [&](TupleCollection nest_collection, TupleCollection prev_collection) {

          for( Tuple nest_access_tuple : nest_collection ){
            for( Tuple previous_access_tuple : prev_collection ){
              cout << nest_access_tuple << " - " << previous_access_tuple << " = ( ";

              bool not_first = false;

              for( Subspace::size_type d = 0; d < dimensions; ++d ){
                // Calculate constant lower bound
                int constant = nest_access_tuple[d] - previous_access_tuple[d];
                if( max_on_dims[d] < constant ){
                  max_on_dims[d] = constant;
                }
                // Construct constraint constant <= nest_shift_d - prev_shift
                MPConstraint* constraint = solver.MakeRowConstraint( constant , infinity);
                constraint->SetCoefficient(     (*nest_shifts)[d],  1 );
                constraint->SetCoefficient( (*previous_shifts)[d], -1 );
                constraints.push_back( constraint );

                if( not_first ) cout << ", ";
                not_first = true;
                cout << constant;
              } // for d

              cout << " )" << endl;
            } // for previous_access_tuple
          } // for nest_access_tuple

        }; // lambda function calc_func

        cout << name << endl;
        Dataspace nest_dataspace = nest_dataspaces.find(name)->second;
        Dataspace previous_dataspace = previous_dataspaces.find(name)->second;

        // Writes - writes
        cout << "Writes - Writes" << endl;
        calc_func( nest_dataspace.writes(), previous_dataspace.writes() );

        // Writes - Reads
        cout << "Writes - Reads" << endl;
        calc_func( nest_dataspace.writes(), previous_dataspace.reads() );

        // Reads - Writes
        cout << "Reads - Writes" << endl;
        calc_func( nest_dataspace.reads(), previous_dataspace.writes() );



      } // for( name : common )
    } // for( previous_idx )
  } // for( nest_idx )


  for( int& max : max_on_dims ){
    cout << max << " ";
  }
  cout << endl;

  for( LoopChain::size_type nest_idx = 0; nest_idx < chain.length(); ++nest_idx ){
    std::vector<MPVariable*>* nests_shifts = shift_variables[nest_idx];
    for( Subspace::size_type d = 0; d < dimensions; ++d  ){
      nests_shifts->operator[]( d )->SetUB( max_on_dims[d] );
    }
    for( MPVariable* var : *nests_shifts ){
      cout << var->lb() << " <= " << var->name() << " <= " << var->ub() << endl;
    }
  }


  cout << "Num constraints: " << constraints.size() << endl;
  cout << "Constraints:" << endl;

  for( MPConstraint* constraint : constraints ){
    cout << constraint->lb() << " <= ";
    bool not_first = false;
    for( LoopChain::size_type nest_idx = 0; nest_idx < chain.length(); ++nest_idx ){
      std::vector<MPVariable*>* nests_shifts = shift_variables[nest_idx];
      for( MPVariable* variable : *nests_shifts ){
        if( constraint->GetCoefficient( variable ) != 0.0 ){
          if( not_first ) cout << " + ";
          not_first = true;
          cout << "( " << constraint->GetCoefficient( variable ) << " * "
               << variable->name() << " )";
        }
      }
    }
    cout << endl;
  }


  cout << "coeffecients:" << endl;
  for( LoopChain::size_type nest_idx = 0; nest_idx < chain.length(); ++nest_idx ){
    std::vector<MPVariable*>* nests_shifts = shift_variables[nest_idx];
    bool not_first = false;
    cout << "( ";
    for( MPVariable* v : *nests_shifts ){
      if( not_first ) cout << ", ";
      not_first = true;
      cout << objective->GetCoefficient( v ) << "*" << v->name();
    }
    cout << " )" << endl;
  }

  std::vector<ShiftTransformation*> transformations;
  if( constraints.size() > 0 ){
    objective->SetMinimization();
    cout << "Objective is " << ( objective->minimization()?"":"not ") << " minimization problem" << endl;
    MPSolver::ResultStatus result_status = solver.Solve();

    // Check that the problem has an optimal solution.
    if( result_status != MPSolver::OPTIMAL ) {
      cerr << "The problem does not have an optimal solution!" << endl;
      cerr << "it is ";
      switch( result_status ){
        case MPSolver::FEASIBLE : { cerr << " FEASIBLE." << endl; break; }
        case MPSolver::INFEASIBLE : { cerr << " INFEASIBLE." << endl; break; }
        case MPSolver::UNBOUNDED : { cerr << " UNBOUNDED." << endl; break; }
        case MPSolver::ABNORMAL : { cerr << " ABNORMAL." << endl; break; }
        case MPSolver::MODEL_INVALID : { cerr << " MODEL_INVALID." << endl; break; }
        case MPSolver::NOT_SOLVED : { cerr << " NOT_SOLVED." << endl; break; }
        default:
          cerr << " something??? " << result_status << endl;
      }
    } else {
      cout << "Optimal objective value = " << objective->Value() << endl;

      for( std::map< LoopChain::size_type, std::vector<MPVariable*>* >::iterator it = shift_variables.begin();
            it != shift_variables.end();
            ++it ){
        LoopChain::size_type nest_id = it->first;
        std::vector<MPVariable*>* nests_shifts = it->second;
        std::vector<int> extents;

        cout << nest_id << ": (";
        bool not_first = false;
        for( MPVariable* var : *nests_shifts ){
          if( not_first ) cout << ", ";
          not_first = true;
          cout << var->solution_value();
          extents.push_back( (int) var->solution_value() );
        }
        cout << ")" << endl;

        Tuple tuple( extents );

        if( !tuple.isZeroTuple() ){
          transformations.push_back( new ShiftTransformation( nest_id, tuple ) );
        }
      }
    }
  }

  return transformations;
}
