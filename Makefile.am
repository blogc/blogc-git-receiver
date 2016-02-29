## Autotools settings

if INTERNAL_SQUAREBALL
SUBDIRS = squareball
endif

AM_DISTCHECK_CONFIGURE_FLAGS = \
	--with-squareball=internal \
	$(NULL)

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = autogen.sh LICENSE README.md

bin_PROGRAMS = blogc-git-receiver

blogc_git_receiver_SOURCES = blogc-git-receiver.c

blogc_git_receiver_CFLAGS = \
	$(AM_CFLAGS) \
	$(SQUAREBALL_CFLAGS) \
	$(NULL)

blogc_git_receiver_LDADD = \
	$(SQUAREBALL_LIBS) \
	$(NULL)
