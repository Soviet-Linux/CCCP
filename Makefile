build: 
	[ -d bin ] || mkdir bin
	gcc src/main.c -lspm -O2 -o bin/cccp

debug:
	[ -d bin ] || mkdir bin
	gcc src/main.c -lspm -g -fsanitize=address -o bin/cccp

dev-buildall: install-libspm build

install-libspm:
	cd lib/libspm; echo "Now on libspm"; \
		make  DESTDIR=$(DESTDIR) all; \
		make  DESTDIR=$(DESTDIR) formats; \
		make  DESTDIR=$(DESTDIR) install

install: build
	install -D bin/cccp $(DESTDIR)/usr/bin/cccp
	if ! [ -f $(DESTDIR)/etc/cccp.conf ]; then install -D cccp.conf $(DESTDIR)/etc/cccp.conf; fi

uninstall:
	rm -rf $(DESTDIR)/usr/bin/cccp && rm -rf $(DESTDIR)/etc/cccp.conf && rm -rf $(DESTDIR)/var/cccp/data

clean:
	rm -rf bin/*
