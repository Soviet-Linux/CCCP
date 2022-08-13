OUT = bin/libspm.so
CC = gcc
ODIR = obj
SDIR = src/libspm

CFLAGS = -Wall -g -fPIC -O2 -Wextra -lsqlite3 -lm 
LIBS = -lcurl

SRCS = $(wildcard $(SDIR)/*.c)
OBJS = $(patsubst %,$(ODIR)/%,$(notdir $(SRCS:.c=.o)))

all: $(OUT)

$(OBJS): $(SRCS)
	$(CC) -c $(INC) -o $@  $(subst .o,.c,$(subst ${ODIR},${SDIR},$@)) $(CFLAGS) 

$(OUT): $(OBJS) 
	gcc -o $@  $(OBJS) $(CFLAGS) $(LIBS) -shared

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(OUT)

