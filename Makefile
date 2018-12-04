CC = clang
CFLAGS = -Wall -pedantic
LDFLAGS = 
DEBUG = -g

all : mush

mush: mush.o parseFuncs.o mushFuncs.o
	$(CC) $(DEBUG) $(LDFLAGS) -o $@ $^

%.o : %.c
	$(CC) $(DEBUG) $(LDFLAGS) -c -o $@ $^

clean : 
	rm -f *.o core
