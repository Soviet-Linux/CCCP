CC = g++
MAIN = main


OUTPUT = bin

FLAGS = -g -std=c++2a -pedantic


default:
	${CC} -c src/make.cpp ${FLAGS} -o ${OUTPUT}/make.o
	${CC} -c src/spm.cpp ${FLAGS} -o ${OUTPUT}/spm.o
	${CC} -c src/pkg.cpp ${FLAGS} -o ${OUTPUT}/pkg.o
	${CC} -c src/data.cpp ${FLAGS} -o ${OUTPUT}/data.o
	${CC} src/${MAIN}.cpp ${FLAGS} ${OUTPUT}/make.o ${OUTPUT}/spm.o ${OUTPUT}/data.o ${OUTPUT}/pkg.o  -o ${OUTPUT}/${MAIN}

run:
	./${OUTPUT}/${MAIN}
clean:
	rm -rf ${OUTPUT}
install:
	cp -r ${OUTPUT}/${MAIN} ${SOVIET}/bin/cccp
