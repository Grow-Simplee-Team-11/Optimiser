##
#	Sample Makefile from the fesif project, needs adjustment to the new directory structure,
#	Author: Archisman Pathak
##
CC = gcc
# CXX = /usr/bin/g++
CXX = /usr/bin/g++-11
OR_LIBS = -L./lib -lortools
CFLAGS = --std=c++17 -W -Wall -Wno-sign-compare -O4 -s -pipe -mmmx -msse -msse2 -msse3 -g -Iinclude/ortools -Iinclude -I. # -mcmodel=medium
# CFLAGS = 
OR_CFLAGS = -v -fPIC -std=c++17 -O4 -DNDEBUG -Iinclude/ortools -Iinclude -I. -DARCH_K8 -Wno-deprecated -DUSE_BOP -DUSE_GLOP -DUSE_CBC -DUSE_CLP -DUSE_SCIP
OR_TOOLS_LNK = -Llib -lortools
LDFLAGS = -Wl,-rpath,@loader_path -Wl,-rpath,@loader_path/../lib -lz -lglog
MEM = -D WATCH_MEM

CLUS_INC_DIR = include/clustering
FESIF_INCLUDE_DIR = $(CLUS_INC_DIR)/fesif
CLUS_SRC_DIR = src/clustering
FESIF_SRC_DIR = $(CLUS_SRC_DIR)/fesif

RP_INC_DIR = include/routeplan
# TSP_OR_INCLUDE_DIR = $(RP_INC_DIR)/
RP_SRC_DIR = src/routeplan


all: main 
#  Build FESIF Code 
global.o: $(FESIF_SRC_DIR)/global.cpp
	$(CXX) $(CFLAGS) -c $(FESIF_SRC_DIR)/global.cpp $(LIBS)
	
HST.o: $(FESIF_SRC_DIR)/HST.cpp $(FESIF_SRC_DIR)/global.cpp global.o
	$(CXX) $(CFLAGS) -c $(FESIF_SRC_DIR)/HST.cpp $(LIBS)
	
utils.o: $(FESIF_INCLUDE_DIR)/utils.h $(FESIF_SRC_DIR)/utils.cpp $(FESIF_INCLUDE_DIR)/HST.h $(FESIF_SRC_DIR)/HST.cpp HST.o $(FESIF_INCLUDE_DIR)/global.h $(FESIF_SRC_DIR)/global.cpp global.o
	$(CXX) $(CFLAGS) -c $(FESIF_SRC_DIR)/utils.cpp $(LIBS)
	
chst: $(FESIF_SRC_DIR)/constructHSTs.cpp $(FESIF_INCLUDE_DIR)/global.h $(FESIF_SRC_DIR)/global.cpp global.o $(FESIF_INCLUDE_DIR)/HST.h $(FESIF_SRC_DIR)/HST.cpp HST.o
	$(CXX) $(CFLAGS) -o chst global.o HST.o $(FESIF_SRC_DIR)/constructHSTs.cpp $(LIBS) $(MEM)

fesif.o:  $(FESIF_SRC_DIR)/FESIF.cpp  $(FESIF_INCLUDE_DIR)/utils.h $(FESIF_SRC_DIR)/utils.cpp utils.o $(FESIF_INCLUDE_DIR)/HST.h $(FESIF_SRC_DIR)/HST.cpp HST.o $(FESIF_INCLUDE_DIR)/global.h $(FESIF_SRC_DIR)/global.cpp global.o
	$(CXX) $(CFLAGS) -c $(FESIF_SRC_DIR)/FESIF.cpp $(LIBS) 

TSP_LK.o:
	$(CXX) $(CFLAGS) -c src/routeplan/TSP_LK.cpp $(LIBS)

# Build TSP Code
TSP_OR.o: 
# $(CXX) -v -c $(RP_SRC_DIR)/TSP_OR.cpp $(CFLAGS) $(OR_CFLAGS) $(LDFLAGS) $(OR_LIBS) $(OR_TOOLS_LNK)  
	/usr/bin/g++-11 -v -c -Iinclude/ortools -Iinclude -I. src/routeplan/TSP_OR.cpp --std=c++17 -W -Wall -Wno-sign-compare -O4 -pipe -mmmx -msse -msse2 -msse3 -g -DNDEBUG -DARCH_K8 -Wno-deprecated -DUSE_BOP -DUSE_GLOP -DUSE_CBC -DUSE_CLP -DUSE_SCIP -lz -lglog -L./lib -Llib -lortools

# Build the EB-AFIT algorithm
EB-AFIT.o:
	$(CXX) $(CFLAGS) -c src/binpack/EB-AFIT.cpp $(LIBS)

Optimiser.o:
	$(CXX) $(CFLAGS) -c src/Optimiser.cpp $(LIBS) 

# Build the executable
main: main.cpp fesif.o TSP_LK.o TSP_OR.o EB-AFIT.o Optimiser.o
	/usr/bin/g++-11 --std=c++17 -W -Wall -Wno-sign-compare -O4 -s -pipe -mmmx -msse -msse2 -msse3 -g -Iinclude/ortools -Iinclude -I.  -o main global.o HST.o utils.o FESIF.o TSP_OR.o EB-AFIT.o TSP_LK.o Optimiser.o main.cpp -L./lib -Llib -lortools


.PHONY: clean
clean:
		-@rm *.o *.gcno *~ 2> /dev/null || true
		-@rm fesif chst 2> /dev/null || true
