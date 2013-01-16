# Makefile for TestFS server
PROG=   1f
SRCS=   1f.c

DPADD+= ${LIBVTREEFS} ${LIBSYS}
LDADD+= -lvtreefs -lsys

CFLAGS+= -D_NETBSD_SOURCE=1

MAN=

BINDIR?= /sbin

.include <bsd.prog.mk>
