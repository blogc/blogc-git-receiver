AC_PREREQ([2.69])

AC_INIT([blogc-git-receiver], [0.1], [https://github.com/blogc/blogc-git-receiver])
AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([1.13 foreign dist-bzip2 dist-xz subdir-objects serial-tests])
AC_CONFIG_HEADERS([config.h])
AM_SILENT_RULES([yes])
AM_MAINTAINER_MODE([enable])

AC_USE_SYSTEM_EXTENSIONS

AC_PROG_CC_C99
AS_IF([test "x$ac_cv_prog_cc_c99" = "xno"], [
  AC_MSG_ERROR([no C99 compiler found, blogc-git-receiver requires a C99 compiler.])
])

AC_CHECK_HEADERS([libgen.h unistd.h errno.h sys/stat.h sys/types.h dirent.h time.h], [],
  [unable to find some required header])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

AS_ECHO("
        ==== ${PACKAGE_STRING} ====

        prefix:       ${prefix}
        exec_prefix:  ${exec_prefix}
        bindir:       ${bindir}

        compiler:     ${CC}
        cflags:       ${CFLAGS}
        ldflags:      ${LDFLAGS}
")
