COMPILER_TYPE= gnu

CFLAGS=-std=c99
CC=gcc

PROG=	snakex3
SRCS=	snakex3.c game.c menu.c timer.c keyboard.c mouse.c vbe.c video_gr.c read_xpm.c RTC.c Snake.c MiscData.c bitmap.c

CCFLAGS= -Wall



DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

BINDIR?= /usr/sbin

.include <bsd.prog.mk>
.include <bsd.gcc.mk>