build:
	[ -d bin ] || mkdir bin
	gcc src/main.c -lspm -Ofast -o bin/cccp 

clean:
	rm -rf bin/*

install: build
	cp bin/cccp /usr/bin