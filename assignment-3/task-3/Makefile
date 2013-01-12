# Makefile for TestFS server
PROG=   rssfs
SRCS=   rssfs.c bencode.c

DPADD+= ${LIBVTREEFS} ${LIBSYS}
LDADD+= -lvtreefs -lsys

CFLAGS+= -D_NETBSD_SOURCE=1

MAN=
BINDIR?= /sbin

.include <bsd.prog.mk>
