########################################

AM_CFLAGS=$(CWFLAG) \
	-I$(includedir) -D_REENTRANT -DLIBC_SYSCALLS -D_GNU_SOURCE


library_includedir="$(includedir)"

TESTS = test_url
check_PROGRAMS = $(TESTS)

LDADD = ../src/.libs/libdcap.la
AM_LDFLAGS = $(CUNIT_LIBS)

test_url_SOURCES = test_url.c

