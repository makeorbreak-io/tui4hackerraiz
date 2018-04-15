CFLAGS=-lncurses -lpanel -lmenu
CC=cc_args.py g++
OBJFLAG=-c
GRAPHICS=graphics.cpp
LIBFILES=Window.cpp Menu.cpp EntryText.cpp WindowManager.cpp
LIBOBJ=Window.o Menu.o EntryText.o WindowManaget.o
DEBUG=-DDEBUG_

libs:
	# $(CC) $(GRAPHICS) $(OBJFLAG)
	$(CC) $(LIBFILES) $(OBJFLAG) $(CFLAGS)

debuglibs:
	$(CC) $(DEBUG) $(LIBFILES) $(OBJFLAG) $(CFLAGS)

clean:
	rm -f $(LIBOBJ)
	rm -f *.*.gch

