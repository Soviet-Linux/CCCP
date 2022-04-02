if [ -z "$var" ]
then
    g++ src/main.cpp -g -pedantic -o bin/build && bin/build
else
    g++ $1 -g -pedantic -o bin/test && bin/test;
fi

