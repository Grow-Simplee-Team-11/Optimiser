##
#	Sample Makefile from the Grow_Simplee project, needs adjustment to the new directory structure,
#	Author: Aniket Kumar
##

CC = gcc
CXX = g++
LIBS = 
CFLAGS = --std=c++17 -W -Wall -Wno-sign-compare -O3 -s -lm -pipe -mmmx -msse -msse2 -msse3 -g # -mcmodel=medium
MEM = -D WATCH_MEM
CLUS_INC_DIR = include/interface
CLARKE_INCLUDE_DIR = src/clustering/Clarke

Clarke : main.o clarke.o
	$(CXX) $(CFLAGS) main.o clarke.o -o Clarke

clarke.o : $(CLARKE_INCLUDE_DIR)/clarke.cpp
	$(CXX) $(CFLAGS) -c $(CLARKE_INCLUDE_DIR)/clarke.cpp $(LIBS)

main.o : main.cpp
	$(CXX) $(CFLAGS) -c main.cpp $(LIBS)

clean :
	rm *.o Clarke


