CFLAGS=-lncurses -lpanel -lmenu
CC=cc_args.py g++
OBJFLAG=-c
LIBFILES=Window.cpp
LIBOBJ=*.o
LIBSDIR=libs
DEBUG=-DDEBUG_
MAINFILE=main.cpp
exec=main

all:main

libs:
	make -C libs/ libs

main:libs
	$(CC) $(LIBSDIR)/$(LIBOBJ) $(MAINFILE) -o ../$(exec) $(CFLAGS)
	rm -f $(LIBSIDR)/*.*.gch

debuglibs:
	make -C libs/ debuglibs

debug:debuglibs
	$(CC) $(LIBSDIR)/$(LIBOBJ) $(MAINFILE) -o ../$(exec) $(CFLAGS)
	rm -f $(LIBSIDR)/*.*.gch

clean:
	make -C $(LIBSDIR)/ clean

