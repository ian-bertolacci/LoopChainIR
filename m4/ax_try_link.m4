# SYNOPSIS
#
# AX_TRY_LINK(library, includes, function-body [, action-if-true [, action-if-false]])
#
# DESCRIPTION
#
# This function is a wrapper around AC_ARG_WITH, which adds -I"value" to CPPFLAGS.
# "--with-" variable is initialized to default value, if it is passed.
#
AC_DEFUN([AX_TRY_LINK], [
    dnl Below logic is a workaround for the limitation, that variables may not allow
    dnl symbols like "+" or "-". See AC_CHECK_LIB source comments for more information.
    m4_ifval([$4], , [AH_CHECK_LIB([$1])])
    AS_LITERAL_IF([$1],
        [AS_VAR_PUSHDEF([ac_Lib], [ac_cv_lib_$1_$2])],
        [AS_VAR_PUSHDEF([ac_Lib], [ac_cv_lib_$1''_$2])])

    AC_CACHE_CHECK([for -l$1], [ac_Lib], [
        dnl Save the current state
        AC_LANG_SAVE
        AC_LANG_CPLUSPLUS
        ax_try_link_save_LIBS=$LIBS
        LIBS="-l$1 $LIBS"

        AC_TRY_LINK([$2], [$3], [AS_VAR_SET([ac_Lib], [yes])], [AS_VAR_SET([ac_Lib], [no])])

        dnl Restore the state to original regardless to the result
        LIBS=$ax_try_link_save_LIBS
        AC_LANG_RESTORE
    ])

    dnl If the variable is set, we define a constant and push library to LIBS by default or execute $4, otherwise execute $5.
    AS_VAR_IF([ac_Lib], [yes],
        [m4_default([$4], [
            AC_DEFINE_UNQUOTED(AS_TR_CPP(HAVE_LIB$1))
            dnl Do not prepend a library, if it is already in the list:
            (echo $LIBS | grep -q -- "-l$1 ") || LIBS="-l$1 $LIBS"
        ])],
        [$5]
    )
    AS_VAR_POPDEF([ac_Lib])
]) # AX_ARG_WITH
