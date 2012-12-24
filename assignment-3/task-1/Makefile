# Makefile for TestFS server
PROG=   testfs
SRCS=   testfs.c

DPADD+= ${LIBVTREEFS} ${LIBSYS}
LDADD+= -lvtreefs -lsys

CFLAGS+= -D_NETBSD_SOURCE=1

MAN=

BINDIR?= /sbin

.include <bsd.prog.mk>
