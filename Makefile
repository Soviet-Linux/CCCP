build:
	[ -d bin ] || mkdir bin
	gcc src/main.c -lspm -O2 -o bin/cccp 

clean:
	rm -rf bin/*

install: build
	cp bin/cccp /usr/bin
	echo -e "SOVIET_REPOS=https://raw.githubusercontent.com/Soviet-Linux/OUR/main/\nSOVIET_FORMATS=ecmp" | tee /etc/cccp.conf > /dev/null
