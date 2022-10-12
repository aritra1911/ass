CC		?=	/usr/bin/cc
CFLAGS		=	-std=iso9899:1999 -pedantic -pedantic-errors -Wall -Wextra \
			-Werror-implicit-function-declaration \
			-Wno-unknown-warning-option -Wno-reserved-id-macro \
			-Wno-reserved-identifier -Wno-disabled-macro-expansion \
			-Wno-missing-prototypes -Wno-thread-safety-analysis \
			-Wno-padded -Wno-cast-align -Wno-comma \
			-g -O0 -fno-fast-math -fno-builtin
CPPFLAGS	+=	-I. -I/usr/local/include -I/usr/pkg/include \
			-D_XOPEN_SOURCE=600 -D_POSIX_PTHREAD_SEMANTICS \
			-D_LARGEFILE64_SOURCE
LDFLAGS		=	-L/usr/local/lib -L/usr/pkg/lib \
			-Wl,-R/usr/local/lib -Wl,-R/usr/pkg/lib

TARGET		?=	ass
OBJS	    	=       $(SRCS:.c=.o)

.PHONY:	all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJS)
