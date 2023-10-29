build:
	[ -d bin ] || mkdir bin
	gcc src/main.c -lspm -o bin/cccp 

clean:
	rm -rf bin/*
