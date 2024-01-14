ARCH="`uname -s`"

ifeq ($(OS),Windows_NT)
    # Windows平台的操作
    TARGET = myprogram.exe
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        # Linux平台的操作
        TARGET = myprogram
    endif
endif
CFLAG:= -lfreeglut -lglew32 -lopengl32 -lglfw3

all: main.cpp glad.c
	gcc  -o glad.o   -c glad.c
	g++ -o main.o -c main.cpp  
	g++     glad.o main.o  -o test $(CFLAG)


# all: $(TARGET)

# $(TARGET): glad.o main.o
# 	g++ $^ -o $@ $(CFLAG)

# glad.o: glad.c
# 	gcc -c $< -o $@

# main.o: main.cpp
# 	g++ -c $< -o $@

# a.exe:main.cpp
# g++ -m32 -std=c++11 main.cpp -static -I ../\dep\GLFW\include -L ../\dep\GLFW\lib-mingw-w64 -lglfw3 -lgdi32 -luser32 -lkernel32 -lopengl32
#
