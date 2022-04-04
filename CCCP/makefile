CC = g++
MAIN = main


OUTPUT = bin

FLAGS = -g -ansi -pedantic

default:
	${CC} src/${MAIN}.cpp ${FLAGS} -o ${OUTPUT}/${MAIN} && ./${OUTPUT}/${MAIN}
build:
	${CC} src/${MAIN}.cpp ${FLAGS} -o ${OUTPUT}/${MAIN}
run:
	./${OUTPUT}/${MAIN}
clean:
	rm -rf ${OUTPUT}