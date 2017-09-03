# Linux
CC=g++
LIBS= -lglut -lGLEW -lGL

# OSX
#CC=clang++
#LIBS=  -framework OpenGL -framework GLUT -framework Cocoa -lGLEW

# Compiler flags
CXXFLAGS= -g -Wall -std=c++0x

../bin/Shader: ../build/Driver.o 
	$(CC) $(CXXFLAGS) ../build/Driver.o -o ../bin/Shader $(LIBS)

../build/Driver.o: ../src/Driver.cpp
	$(CC) $(CXXFLAGS) -c ../src/Driver.cpp $(LIBS)

