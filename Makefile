##
#	Sample Makefile from the Grow_Simplee project, needs adjustment to the new directory structure,
#	Author: Aniket Kumar
##

CC = gcc
CXX = g++
LIBS = 
CFLAGS = --std=c++17 -W -Wall -Wno-sign-compare -O3 -s -lm -pipe -mmmx -msse -msse2 -msse3 -g # -mcmodel=medium
CLARKE_INCLUDE_DIR = src/clustering/Clarke
TSP_INCLUDE_DIR = src/routeplan
BP_INCLUDE_DIR = src/binpack

all: Integrate 

Integrate: main.cpp $(CLARKE_INCLUDE_DIR)/clarke.cpp $(CLARKE_INCLUDE_DIR)/clarke.hpp $(TSP_INCLUDE_DIR)/TSP.cpp $(TSP_INCLUDE_DIR)/tsp.h $(BP_INCLUDE_DIR)/EB_AFIT.cpp $(BP_INCLUDE_DIR)/EB_AFIT.h
	$(CXX) $(CFLAGS) main.cpp $(CLARKE_INCLUDE_DIR)/clarke.cpp $(TSP_INCLUDE_DIR)/TSP.cpp $(BP_INCLUDE_DIR)/EB_AFIT.cpp -o Integrate

clean :
	rm Integrate


