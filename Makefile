lib:
	cd lib/libspm && \
	make DESTDIR=$(DESTDIR) all && \
	make DESTDIR=$(DESTDIR) formats && \
	make DESTDIR=$(DESTDIR) install

build: lib
	[ -d bin ] || mkdir -p bin && \
	gcc src/main.c -lspm -O2 -o bin/cccp

all: build install clean

buildnolib:
	[ -d bin ] || mkdir -p bin
	gcc src/main.c -lspm -O2 -o bin/cccp -L$(DESTDIR)/usr/lib -I$(DESTDIR)/usr/include

clean:
	rm -rf bin/*

install: build
	cp bin/cccp $(DESTDIR)/bin
	cp cccp.conf $(DESTDIR)/etc/cccp.conf

uninstall:
	rm -rf $(DESTDIR)/bin/cccp && rm -rf $(DESTDIR)/etc/cccp.conf && rm -rf $(DESTDIR)/var/cccp/data
