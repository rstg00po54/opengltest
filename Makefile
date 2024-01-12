all:main.cpp glad.c
	/usr/bin/cc  -o glad.o   -c glad.c
	/usr/bin/c++ -o main.o -c main.cpp
	/usr/bin/c++    -rdynamic glad.o main.o  -o test  /usr/local/lib/libglfw3.a -lpthread -lrt -lm -ldl