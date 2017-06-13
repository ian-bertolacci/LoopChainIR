AC_INIT(myconfig, version-0.1)

dnl Set language to C++
AC_LANG(C++)
AC_PROG_CXX

dnl set CXXFLAGS
CXXFLAGS="-O3 -g -DUSE_CBC -DUSE_CLP -DUSE_GLOP -Wall -Wno-deprecated -Wextra --std=c++11 -fPIC -I\$(INCLUDE)"

dnl Check existance of libraries
dnl Check for ISL
dnl Check for ISL headers
AC_CHECK_HEADERS([isl/version.h], [], [AC_ERROR([Missing isl/version.h. A Working Integer Set Library (ISL) is required])])
dnl Check for ISL Library
AC_SEARCH_LIBS(isl_version, isl)

dnl Check for software
dnl Check and set python
dnl TODO Better way of doing this.
AC_SUBST( PYTHON, missing )
AS_IF([test x"$PYTHON" == x"missing"],
	AC_CHECK_PROG( _PYTHON2_maybe, python2, [python2], [missing], )
	AS_IF([test x"$_PYTHON2_maybe" != x"missing"],
		AC_SUBST( PYTHON, $_PYTHON2_maybe )
	)
)
AS_IF([test x"$PYTHON" == x"missing"],
	AC_CHECK_PROG( _PYTHON_maybe, python, [python], [missing], )
	AS_IF([test x"$_PYTHON_maybe" != x"missing"],
		AC_SUBST( PYTHON, $_PYTHON_maybe )
	)
)
AS_IF([test x"$PYTHON" == x"missing"],
	AC_CHECK_PROG( _PYPY_maybe, pypy, [pypy], [missing], )
	AS_IF([test x"$_PYPY_maybe" != x"missing"],
		AC_SUBST( PYTHON, $_PYPY_maybe )
	)
)

dnl Check for unzip
AC_SUBST( UNZIP, missing )
AS_IF([test x"$UNZIP" == x"missing"],
	AC_CHECK_PROG( _UNZIP_maybe, unzip, [unzip], [missing], )
	AS_IF([test x"$_UNZIP_maybe" != x"missing"],
		AC_SUBST( UNZIP, $_UNZIP_maybe )
	)
)

dnl Process Makefile.in to create Makefile
AC_OUTPUT(Makefile)
