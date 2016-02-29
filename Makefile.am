## Autotools settings

if INTERNAL_SQUAREBALL
SUBDIRS = squareball
endif

AM_DISTCHECK_CONFIGURE_FLAGS = \
	--with-squareball=internal \
	$(NULL)

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = \
	build-aux/git-version-gen \
	$(top_srcdir)/.version \
	autogen.sh \
	LICENSE \
	README.md \
	$(NULL)

BUILT_SOURCES = \
	$(top_srcdir)/.version \
	$(NULL)

bin_PROGRAMS = blogc-git-receiver

blogc_git_receiver_SOURCES = blogc-git-receiver.c

blogc_git_receiver_CFLAGS = \
	$(AM_CFLAGS) \
	$(SQUAREBALL_CFLAGS) \
	$(NULL)

blogc_git_receiver_LDADD = \
	$(SQUAREBALL_LIBS) \
	$(NULL)

$(top_srcdir)/.version:
	echo $(VERSION) > $@-t && mv $@-t $@

dist-hook:
	echo $(VERSION) > $(distdir)/.tarball-version
