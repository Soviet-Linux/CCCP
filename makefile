CC = g++
MAIN = main
TEST = test
TARGET = null

OUTPUT = bin

FLAGS = -g -std=c++2a -pedantic


default:

	${CC} src/${MAIN}.cpp ${FLAGS} ${OUTPUT}/make.o ${OUTPUT}/spm.o ${OUTPUT}/data.o ${OUTPUT}/pkg.o  -o ${OUTPUT}/${MAIN}

run:
	./${OUTPUT}/${MAIN}
clean:
	rm -rf ${OUTPUT}
install:
	cp -r ${OUTPUT}/${MAIN} ${SOVIET}/bin/cccp
test :
	${CC} -c src/${TARGET}.cpp ${FLAGS} -o ${OUTPUT}/${TARGET}.o
	${CC} ${OUTPUT}/${TARGET}.o tests/${TEST}.cpp ${FLAGS} -o ${OUTPUT}/${TEST}
	./${OUTPUT}/${TEST}
