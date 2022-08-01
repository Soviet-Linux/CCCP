
all:
	mkdir bin &
	mkdir obj &
	./make.sh lib
	./make.sh cccp
libspm:
	./make.sh lib
cccp:
	./make.sh cccp
install:
	cp bin/libspm.so $(DESTDIR)/usr/lib/libspm.so
	cp bin/cccp $(DESTDIR)/usr/bin/cccp
	cp scripts/* $(DESTDIR)/usr/bin
	echo REPOS=http://our.sovietlinux.ml > $(DESTDIR)/etc/cccp.conf

uninstal:
	./make.sh uninstall
test:
	./make.sh test
clean:
	./make.sh clean
