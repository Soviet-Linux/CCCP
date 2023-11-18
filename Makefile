all: build install

build:
	cd lib/libspm
	make all
	make formats
	sudo make install
	cd ../..
	[ -d bin ] || mkdir bin
	gcc src/main.c -lspm -O2 -o bin/cccp 

buildnolib: 
	[ -d bin ] || mkdir bin
	gcc src/main.c -lspm -O2 -o bin/cccp	

clean:
	rm -rf bin/*

install: build
	cp bin/cccp /usr/bin
	echo -e "SOVIET_REPOS=https://raw.githubusercontent.com/Soviet-Linux/OUR/main\nSOVIET_FORMATS=ecmp" | tee /etc/cccp.conf > /dev/null
