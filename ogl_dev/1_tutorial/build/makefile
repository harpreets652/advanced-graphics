# Linux
CC=g++
LIBS= -lglut -lGLEW -lGL

# OSX
#CC=clang++
#LIBS=  -framework OpenGL -framework GLUT -framework Cocoa -lGLEW

# Compiler flags
CXXFLAGS= -g -Wall -std=c++0x

../bin/Window: ../build/Window.o
	$(CC) $(CXXFLAGS) ../build/Window.o -o ../bin/Window $(LIBS)

../build/Window.o: ../src/Window.cpp
	$(CC) $(CXXFLAGS) -c ../src/Window.cpp $(LIBS)

