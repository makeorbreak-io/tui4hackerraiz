
exec=main

all:main

libs:
	make -C src/libs libs

main:libs
	make -C src/ main

debuglibs:
	make -C src/libs debuglibs

debug:debuglibs
	make -C src/ debug

clean:
	make -C src/libs clean
	make -C src/ clean
	rm -f main

run:main
	./main

rundebug:debug
	./main
