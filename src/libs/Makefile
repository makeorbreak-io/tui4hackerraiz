CFLAGS=-lncurses -lpanel -lmenu
CC=cc_args.py g++
OBJFLAG=-c
GRAPHICS=graphics.cpp
GRAPHICSOBJ=graphics.o
LIBFILES=$(GRAPHICSOBJ) Window.cpp Menu.cpp
# LIBFILES=$(GRAPHICSOBJ) Window.cpp
# LIBOBJ=$(GRAPHICSOBJ) Window.o
LIBOBJ=$(GRAPHICSOBJ) Window.o Menu.o
DEBUG=-DDEBUG_

libs:
	$(CC) $(GRAPHICS) $(OBJFLAG)
	$(CC) $(LIBFILES) $(OBJFLAG) $(CFLAGS)

debuglibs:
	$(CC) $(DEBUG) $(LIBFILES) $(OBJFLAG) $(CFLAGS)

clean:
	rm -f $(LIBOBJ)
	rm -f *.*.gch

