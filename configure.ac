AC_PREREQ([2.69])

AC_INIT([blogc-git-receiver], m4_esyscmd([build-aux/git-version-gen .tarball-version]),
        [https://github.com/blogc/blogc-git-receiver], [blogc-git-receiver],
        [https://blogc.rgm.io/])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([1.13 foreign dist-bzip2 dist-xz subdir-objects serial-tests])
AC_CONFIG_HEADERS([config.h])
AM_SILENT_RULES([yes])
AM_MAINTAINER_MODE([enable])

AC_USE_SYSTEM_EXTENSIONS

LT_INIT

AC_PROG_CC_C99
AS_IF([test "x$ac_cv_prog_cc_c99" = "xno"], [
  AC_MSG_ERROR([no C99 compiler found, blogc-git-receiver requires a C99 compiler.])
])

AC_PROG_SED

RPM_VERSION=`echo ${PACKAGE_VERSION} | sed "s/-/./g"`
AC_SUBST(RPM_VERSION)

AC_ARG_ENABLE([rpmbuild], AS_HELP_STRING([--disable-rpmbuild],
              [ignore presence of rpmbuild and disable srpm generation]))
AS_IF([test "x$enable_rpmbuild" != "xno"], [
  AC_PATH_PROG([rpmbuild], [rpmbuild])
  AS_IF([test "x$ac_cv_path_rpmbuild" = "x"], [
    have_rpmbuild=no
  ], [
    have_rpmbuild=yes
  ])
])
AS_IF([test "x$have_rpmbuild" = "xyes"], , [
  AS_IF([test "x$enable_rpmbuild" = "xyes"], [
    AC_MSG_ERROR([rpmbuild requested but not found])
  ])
])
AM_CONDITIONAL([USE_RPMBUILD], [test "x$ac_cv_path_rpmbuild" != "x"])
RPMBUILD="$ac_cv_path_rpmbuild"
AC_SUBST(RPMBUILD)

AC_CHECK_HEADERS([libgen.h unistd.h errno.h sys/stat.h sys/types.h dirent.h time.h], , [
  AC_MSG_ERROR([unable to find some required header])
])

AC_ARG_WITH([squareball], [AS_HELP_STRING([--with-squareball=@<:@internal/system@:>@],
            [whether to use library squareball from system [default=internal]])])
AS_IF([test "x$with_squareball" = "xsystem"], [
  SQUAREBALL="system"
  PKG_CHECK_MODULES([SQUAREBALL], [squareball >= 0.2.0], , [
    AC_MSG_ERROR([library squareball requested from system but not found or not new enough])
  ])
], [
  SQUAREBALL="internal"
  SQUAREBALL_CFLAGS='-I$(top_srcdir)/squareball/src'
  SQUAREBALL_LIBS='$(top_builddir)/squareball/libsquareball.la'
  AC_SUBST(SQUAREBALL_LIBS)
  AC_SUBST(SQUAREBALL_CFLAGS)
  ac_configure_args_pre="$ac_configure_args"
  ac_configure_args_post="$ac_configure_args --enable-bundleme"
  ac_configure_args="$ac_configure_args_post"
  AC_CONFIG_COMMANDS_PRE([ac_configure_args="$ac_configure_args_pre"])
  AC_CONFIG_COMMANDS_POST([ac_configure_args="$ac_configure_args_post"])
  AC_CONFIG_SUBDIRS([squareball])
  ac_configure_args="$ac_configure_args_pre"
])
AM_CONDITIONAL(INTERNAL_SQUAREBALL, [test "x$with_squareball" != "xsystem"])

AC_CONFIG_FILES([
  Makefile
  blogc-git-receiver.spec
])
AC_OUTPUT

AS_ECHO("
        ==== ${PACKAGE_STRING} ====

        prefix:       ${prefix}
        exec_prefix:  ${exec_prefix}
        bindir:       ${bindir}

        compiler:     ${CC}
        cflags:       ${CFLAGS}
        ldflags:      ${LDFLAGS}

        squareball:   ${SQUAREBALL}

        rpmbuild:     ${RPMBUILD}
")
