AC_INIT([LoopChainIR],[version-0.1])

m4_include([m4/ax_require_defined.m4])
m4_include([m4/ax_cxx_compile_stdcxx.m4])
m4_include([m4/ax_cxx_compile_stdcxx_11.m4])
m4_include([m4/ax_cxx_check_lib.m4])
m4_include([m4/ax_try_link.m4])

dnl Set language to C++
AC_LANG([C++])
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX_11([noext])

dnl set CXXFLAGS
CXXFLAGS="-O3 -g -DUSE_CBC -DUSE_CLP -DUSE_GLOP -Wall -Wno-deprecated -Wextra --std=c++11 -fPIC -I\$(INCLUDE)"

dnl Check existance of libraries
dnl Check for ISL
dnl Check for ISL headers
AC_CHECK_HEADERS(
  [isl/version.h],
  [],
  [AC_MSG_ERROR(Missing isl/version.h. A Working Integer Set Library (ISL) is required.)]
)
dnl Check for ISL Library
AC_SEARCH_LIBS(
  isl_version,
  isl,
  [],
  [AC_MSG_ERROR(Cannot find isl_version in libisl. A Working Integer Set Library (ISL) is required.)]
)
dnl Check for OR-Tools
dnl Check for OR-Tools headers
AC_CHECK_HEADERS(
  [linear_solver/linear_solver.h],
  [],
  [AC_MSG_ERROR(Missing linear_solver/linear_solver.h. A Working Google Optimization Tools (Google OR-Tools) is required.)]
)
dnl Check for OR-Tools Library
AX_TRY_LINK(
  [ ortools ],
  [ #include <linear_solver/linear_solver.h> #include <string> ],
  [
    #if defined USE_SCIP
      operations_research::MPSolver::OptimizationProblemType optimizationProblemType = operations_research::MPSolver::SCIP_MIXED_INTEGER_PROGRAMMING;
    #elif defined USE_GLPK
      operations_research::MPSolver::OptimizationProblemType optimizationProblemType = operations_research::MPSolver::GLPK_MIXED_INTEGER_PROGRAMMING;
    #elif defined USE_CBC
      operations_research::MPSolver::OptimizationProblemType optimizationProblemType = operations_research::MPSolver::CBC_MIXED_INTEGER_PROGRAMMING;
    #elif defined USE_SLM
      operations_research::MPSolver::OptimizationProblemType optimizationProblemType = operations_research::MPSolver::SULUM_MIXED_INTEGER_PROGRAMMING;
    #elif defined USE_GUROBI
      operations_research::MPSolver::OptimizationProblemType optimizationProblemType = operations_research::MPSolver::GUROBI_MIXED_INTEGER_PROGRAMMING;
    #elif defined USE_CPLEX
      operations_research::MPSolver::OptimizationProblemType optimizationProblemType = operations_research::MPSolver::CPLEX_MIXED_INTEGER_PROGRAMMING
    #elif defined USE_BOP
      operations_research::MPSolver::OptimizationProblemType optimizationProblemType = operations_research::MPSolver::BOP_INTEGER_PROGRAMMING;
    #else
      #error "Google Optimization Tools has no proper backend!"
    #endif

    operations_research::MPSolver( std::string("test"), optimizationProblemType );
  ],
  [],
  [AC_MSG_ERROR(Cannot find MPSolver in libortools. A Working Google Optimization Tools (Google OR-Tools) is required.)],
)

dnl Check for software
dnl Check and set python
dnl TODO Better way of doing this.
AC_SUBST( PYTHON, missing )
AS_IF(
  [test x"$PYTHON" == x"missing"],
  AC_CHECK_PROG(
    _PYTHON2_maybe,
    python2,
    [python2],
    [missing]
  )
  AS_IF(
    [test x"$_PYTHON2_maybe" != x"missing"],
    AC_SUBST( PYTHON, $_PYTHON2_maybe )
  )
)
AS_IF([test x"$PYTHON" == x"missing"],
  AC_CHECK_PROG(
    _PYTHON_maybe,
    python,
    [python],
    [missing]
  )
  AS_IF(
    [test x"$_PYTHON_maybe" != x"missing"],
    AC_SUBST( PYTHON, $_PYTHON_maybe )
  )
)
AS_IF(
  [test x"$PYTHON" == x"missing"],
  AC_CHECK_PROG( _PYPY_maybe, pypy, [pypy], [missing], )
  AS_IF([test x"$_PYPY_maybe" != x"missing"],
    AC_SUBST( PYTHON, $_PYPY_maybe )
  )
)

dnl Check for unzip
AC_SUBST( UNZIP, missing )
AS_IF(
  [test x"$UNZIP" == x"missing"],
  AC_CHECK_PROG(
    _UNZIP_maybe,
    unzip,
    [unzip],
    [missing]
  )
  AS_IF([test x"$_UNZIP_maybe" != x"missing"],
    AC_SUBST( UNZIP, $_UNZIP_maybe )
  )
)

dnl Process Makefile.in to create Makefile
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
