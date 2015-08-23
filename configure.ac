AC_PREREQ([2.69])

AC_INIT([blogc-git-receiver], [0], [https://github.com/blogc/blogc-git-receiver])
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

AC_PATH_PROG([git], [git])
AS_IF([test "x$ac_cv_path_git" = "x"], [
  AC_MSG_ERROR([git not found.])])
AC_DEFINE_UNQUOTED([GIT_BINARY], ["$ac_cv_path_git"],
  [git binary path])

AC_PATH_PROG([git_shell], [git-shell])
AS_IF([test "x$ac_cv_path_git_shell" = "x"], [
  AC_MSG_ERROR([git-shell not found.])])
AC_DEFINE_UNQUOTED([GIT_SHELL_BINARY], ["$ac_cv_path_git_shell"],
  [git-shell binary path])

AC_PATH_PROGS([gmake], [gmake make])
AS_IF([test "x$ac_cv_path_gmake" = "x"], [
  AC_MSG_ERROR([gmake not found.])])
AC_DEFINE_UNQUOTED([GMAKE_BINARY], ["$ac_cv_path_gmake"],
  [gmake binary path])

AC_PATH_PROG([tar], [tar])
AS_IF([test "x$ac_cv_path_tar" = "x"], [
  AC_MSG_ERROR([tar not found.])])
AC_DEFINE_UNQUOTED([TAR_BINARY], ["$ac_cv_path_tar"],
  [tar binary path])

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
