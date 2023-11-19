all: build install clean

build:
	cd lib/libspm; echo "Now on libspm"; \
        make all; \
		make formats; \
		make install 
	cd ../.. && echo "Now on CCCP"
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

uninstall:
	rm -rf /usr/bin/cccp && rm -rf /etc/cccp.conf && rm -rf /var/cccp/data
