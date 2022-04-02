CC = g++
MAIN = main


OUTPUT = output

FLAGS = -g -ansi -pedantric

default:
    ${CC} src/${MAIN}.cpp ${FLAGS} -o /${OUTPUT}/${MAIN} && ./${OUTPUT}/${MAIN}
build:
	${CC} src/${MAIN}.cpp ${FLAGS} -o /${OUTPUT}/${MAIN}
run:
./${OUTPUT}/${MAIN}
clean:
	rm -rf ${OUTPUT}