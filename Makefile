SRCS	=	main.c arm.c strtrim.c
OBJS	=	$(SRCS:.c=.o)

.PHONY:	all clean

all: ass

ass: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

clean:
	rm -f ass $(OBJS)
