build: 
	[ -d bin ] || mkdir bin
	gcc src/main.c -lspm -O2 -o bin/cccp	

dev-buildall: install-libspm build

install-libspm:
	cd lib/libspm; echo "Now on libspm"; \
		make  DESTDIR=$(DESTDIR) all; \
		make  DESTDIR=$(DESTDIR) formats; \
		make  DESTDIR=$(DESTDIR) install
    
install: build
	install -D bin/cccp /usr/bin
	install -D cccp.conf /etc/cccp.conf

uninstall:
	rm -rf /usr/bin/cccp && rm -rf /etc/cccp.conf && rm -rf /var/cccp/data

clean:
	rm -rf bin/*