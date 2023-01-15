##
#	Sample Makefile from the Grow_Simplee project, needs adjustment to the new directory structure,
#	Author: Aniket Kumar
##

CC = gcc
CXX = g++
LIBS = 
CFLAGS = --std=c++17 -W -Wall -Wno-sign-compare -O3 -s -lm -pipe -mmmx -msse -msse2 -msse3 -g # -mcmodel=medium
MEM = -D WATCH_MEM
CLARKE_INCLUDE_DIR = src/clustering/Clarke
TSP_INCLUDE_DIR = src/routeplan

all: Integrate 

Integrate: main.cpp $(CLARKE_INCLUDE_DIR)/clarke.cpp $(CLARKE_INCLUDE_DIR)/clarke.hpp $(TSP_INCLUDE_DIR)/TSP.cpp $(TSP_INCLUDE_DIR)/tsp.h
	$(CXX) $(CFLAGS) main.cpp $(CLARKE_INCLUDE_DIR)/clarke.cpp $(TSP_INCLUDE_DIR)/TSP.cpp -o Integrate

clean :
	rm Integrate


