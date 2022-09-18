LIBOUT = bin/libspm.so
EXEOUT = bin/cccp

CC = gcc
CPP = g++

ODIR = obj
SDIR = src/libspm
CPPDIR = src/cccp/cpp
RSDIR = src/cccp/rust

CFLAGS = -Wall -g -fPIC -O2 -Wextra 
RSFLAGS = -O
LIBS = -lcurl -lsqlite3 -lm 

SRCS = $(wildcard $(SDIR)/*.c)
OBJS = $(patsubst %,$(ODIR)/%,$(notdir $(SRCS:.c=.o)))

all: $(LIBOUT) $(EXEOUT)

libspm: $(LIBOUT)

cpp:
	$(CPP) $(CFLAGS) $(CPPDIR)/* -o $(EXEOUT) -lspm -L./bin

rust:
	cargo build --manifest-path $(RSDIR)/Cargo.toml --release
	cp $(RSDIR)/target/release/cccp ./bin/

spm-tester:
	$(CC) $(CFLAGS) src/spm-tester/main.c $(LIBS) -o bin/spm-tester -lspm 

rust-dev:
	cargo build --manifest-path $(RSDIR)/Cargo.toml
	cp $(RSDIR)/target/debug/cccp ./bin/

testing:
	$(CC) $(CFLAGS) tests/test.c $(LIBS) -o tests/bin/test -lspm -L./bin


$(OBJS): ${ODIR}
	$(CC) -c $(INC) -o $@  $(subst .o,.c,$(subst ${ODIR},${SDIR},$@)) $(CFLAGS) 

$(LIBOUT): $(OBJS)
	
	$(CC) -o $@  $(OBJS) $(CFLAGS) $(LIBS) -shared

${ODIR}:
	mkdir -p ${ODIR}
	mkdir -p bin

.PHONY: clean testing

clean:
	rm -f $(ODIR)/*.o $(LIBOUT)
install:
	cp $(LIBOUT) $(DESTDIR)/lib
	cp $(EXEOUT) $(DESTDIR)/bin


