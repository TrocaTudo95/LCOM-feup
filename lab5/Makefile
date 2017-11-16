COMPILER_TYPE= gnu

CFLAGS=-std=c99
CC=gcc

PROG=	lab5
SRCS=	lab5.c test5.c timer.c keyboard.c vbe.c video_gr.c read_xpm.c

CCFLAGS= -Wall



DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

BINDIR?= /usr/sbin

.include <bsd.prog.mk>
.include <bsd.gcc.mk>