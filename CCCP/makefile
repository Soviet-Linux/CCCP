CC = g++
MAIN = main


OUTPUT = bin

FLAGS = -g -std=c++20 -pedantic

default:
	${CC} -c src/make.cpp ${FLAGS} -o ${OUTPUT}/make.o
	${CC} src/${MAIN}.cpp ${FLAGS} ${OUTPUT}/make.o  ${OUTPUT}/misc.o -o ${OUTPUT}/${MAIN} && ./${OUTPUT}/${MAIN}

build:
	${CC} -c src/make.cpp ${FLAGS} -o ${OUTPUT}/make.o
	${CC} src/${MAIN}.cpp ${FLAGS} ${OUTPUT}/make.o  ${OUTPUT}/misc.o -o ${OUTPUT}/${MAIN}

run:
	./${OUTPUT}/${MAIN}

clean:
	rm -rf ${OUTPUT}
