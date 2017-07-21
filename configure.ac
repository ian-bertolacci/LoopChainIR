AC_INIT([LoopChainIR],[version-0.1])

dnl  m4_include([m4/ax_require_defined.m4])
dnl  m4_include([m4/ax_cxx_compile_stdcxx.m4])
dnl  m4_include([m4/ax_cxx_compile_stdcxx_11.m4])
dnl  m4_include([m4/ax_cxx_check_lib.m4])
dnl  m4_include([m4/ax_try_link.m4])

AC_CONFIG_MACRO_DIR([m4])

dnl Set language to C++
AC_LANG([C++])
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX_11([noext])

dnl Create command line flags
dnl Create flags for all linear solver backends
dnl Create --with-ortools-backend flag
dnl TODO Prove that solver is present
OR_TOOLS_USE_SOLVERS=""
AC_ARG_WITH(
  [scip_solver],
  [AS_HELP_STRING(
    [--with-scip-solver],
    [Use scip as the linear solver backend]
  )],
  [OR_TOOLS_USE_SOLVERS+="-DUSE_SCIP "]
)
AC_ARG_WITH(
  [glpk_solver],
  [AS_HELP_STRING(
    [--with-glpk-solver],
    [Use glpk as the linear solver backend]
  )],
  [OR_TOOLS_USE_SOLVERS+="-DUSE_GLPK "]
)
AC_ARG_WITH(
  [cbc_solver],
  [AS_HELP_STRING(
    [--with-cbc-solver],
    [Use cbc as the linear solver backend]
  )],
  [OR_TOOLS_USE_SOLVERS+="-DUSE_CBC "]
)
AC_ARG_WITH(
  [slm_solver],
  [AS_HELP_STRING(
    [--with-slm-solver],
    [Use slm as the linear solver backend]
  )],
  [OR_TOOLS_USE_SOLVERS+="-DUSE_SLM "]
)
AC_ARG_WITH(
  [gurobi_solver],
  [AS_HELP_STRING(
    [--with-gurobi-solver],
    [Use gurobi as the linear solver backend]
  )],
  [OR_TOOLS_USE_SOLVERS+="-DUSE_GUROBI "]
)
AC_ARG_WITH(
  [cplex_solver],
  [AS_HELP_STRING(
    [--with-cplex-solver],
    [Use cplex as the linear solver backend]
  )],
  [OR_TOOLS_USE_SOLVERS+="-DUSE_CPLEX "]
)

dnl TODO Automatically find and determine a solver to use
AS_IF(
  [test x"$OR_TOOLS_USE_SOLVERS" == x""],
  [AC_MSG_NOTICE([No solver specified, defaulting to CBC])]
  OR_TOOLS_USE_SOLVERS="-DUSE_CBC"
)

dnl Create --enable-debugging
AC_ARG_ENABLE(
  [debugging],
  [AS_HELP_STRING(
    [--enable-debugging],
    [Enable gdb debugging options. Default=no]
  )],
  CXX_DEBUG_FLAGS="-ggdb",
  CXX_DEBUG_FLAGS=""
)

dnl Create --enable-rose
AC_ARG_ENABLE(
  [rose],
  [AS_HELP_STRING(
    [--enable-rose],
    [Enable using Rose and Rose dependent parts of library. (Default=yes)]
  )]
)

dnl set CXXFLAGS
CXX_BASE_FLAGS="-fPIC"
CXX_WARNING_LEVEL="-Wall -Wno-deprecated -Wextra"
CXX_OPTIMIZATION_LEVEL="-O3"

CXXFLAGS="${CXX_BASE_FLAGS} ${CXX_WARNING_LEVEL} ${CXX_OPTIMIZATION_LEVEL} ${CXX_DEBUG_FLAGS} ${OR_TOOLS_USE_SOLVERS} ${CXXFLAGS}"

dnl Check existance of libraries
dnl Check for ISL
dnl Check for ISL headers
AC_CHECK_HEADERS(
  [isl/version.h],
  [],
  [AC_MSG_ERROR(A Working Integer Set Library (ISL) installation is required. (Missing isl/version.h.))]
)
dnl Check for ISL Library
AC_SEARCH_LIBS(
  isl_version,
  isl,
  [],
  [AC_MSG_ERROR(A Working Integer Set Library (ISL) installation is required. (Cannot find isl_version in libisl.))]
)
dnl Check for OR-Tools
dnl Check for OR-Tools headers
AC_CHECK_HEADERS(
  [ortools/linear_solver/linear_solver.h],
  [],
  [AC_MSG_ERROR(A Working Google Optimization Tools (Google OR-Tools) installation is required. (Missing ortools/linear_solver/linear_solver.h.))]
)
dnl Check for OR-Tools Library
dnl TODO better way of doing this
AX_TRY_LINK(
  [ortools],
  [ #include <ortools/linear_solver/linear_solver.h> #include <string> ],
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
      operations_research::MPSolver::OptimizationProblemType optimizationProblemType = operations_research::MPSolver::CPLEX_MIXED_INTEGER_PROGRAMMING;
    #else
      #error "Google Optimization Tools has no proper backend!"
    #endif

    operations_research::MPSolver( std::string("test"), optimizationProblemType );
  ],
  [],
  [AC_MSG_ERROR(A Working Google Optimization Tools (Google OR-Tools) installation is required. (Cannot find MPSolver in libortools.))],
)
dnl Check for Rose
dnl Check for Rose headers
AS_IF(
  [test x"$enable_rose" != x"no"],
  [
    AC_CHECK_HEADERS(
      [rose.h],
      [],
      [AC_MSG_ERROR(A Working Rose installation is required. (Missing rose.h.))],
      [
        [ #undef HAVE_SYS_STAT_H ]
        AC_INCLUDES_DEFAULT
      ]
    )
  ]
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
dnl If rose is enabled include the rose dependent sources to be compiled.
AS_IF(
  [test x"$enable_rose" != x"no"],
  [SOURCE_SELECTION="\$(BASE_SRC) \$(ROSE_DEPENDENT_SRC)"],
  [SOURCE_SELECTION="\$(BASE_SRC)"],
)
AC_SUBST(SOURCE_SELECTION, $SOURCE_SELECTION)

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
