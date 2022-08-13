LIBOUT = bin/libspm.so
EXEOUT = bin/cccp

CC = gcc
CPP = g++
RSC = rustc

ODIR = obj
SDIR = src/libspm
CPPDIR = src/cccp/cpp
RSDIR = src/cccp/rust

CFLAGS = -Wall -g -fPIC -O2 -Wextra 
LIBS = -lcurl -lsqlite3 -lm 

SRCS = $(wildcard $(SDIR)/*.c)
OBJS = $(patsubst %,$(ODIR)/%,$(notdir $(SRCS:.c=.o)))

all: $(LIBOUT)

cpp:
	$(CPP) $(CFLAGS) $(CPPDIR)/* -o $(EXEOUT) -lspm -L./bin
rust:
	$(RSC) $(RSDIR)/test.rs -o $(EXEOUT) -lspm -L./bin



$(OBJS): $(SRCS)
	$(CC) -c $(INC) -o $@  $(subst .o,.c,$(subst ${ODIR},${SDIR},$@)) $(CFLAGS) 

$(LIBOUT): $(OBJS) 
	gcc -o $@  $(OBJS) $(CFLAGS) $(LIBS) -shared

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(LIBOUT)
install:
	cp $(LIBOUT) $(DESTDIR)/lib
	cp $(EXEOUT) $(DESTDIR)/bin

