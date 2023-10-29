build:
	[ -d bin ] || mkdir bin
	gcc src/main.c -lspm -O2 -o bin/cccp 

clean:
	rm -rf bin/*

install: build
	cp bin/cccp /usr/bin
	echo -e "REPOS=https://github.com/Soviet-Linux/OUR/blob/main\nFORMATS=ecmp" | sudo tee /etc/cccp.conf > /dev/null
