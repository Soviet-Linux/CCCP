build:
	cd lib/libspm && \
	echo "Now on libspm" && \
	make all && \
	make formats && \
	make install
	cd ../.. && \
	echo "Now on CCCP" && \
	[ -d bin ] || mkdir -p bin && \
	gcc src/main.c -lspm -O2 -o bin/cccp

all: build install clean

buildnolib:
	[ -d bin ] || mkdir -p bin
	gcc src/main.c -lspm -O2 -o bin/cccp -L$(DESTDIR)/lib -I$(DESTDIR)/include

clean:
	rm -rf bin/*

install: build
	cp bin/cccp $(DESTDIR)/bin
	cp cccp.conf $(DESTDIR)/etc/cccp.conf

uninstall:
	rm -rf $(DESTDIR)/bin/cccp && rm -rf $(DESTDIR)/etc/cccp.conf && rm -rf $(DESTDIR)/var/cccp/data
