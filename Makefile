##
#	Sample Makefile from the fesif project, needs adjustment to the new directory structure,
#	Author: Archisman Pathak & Aniket Kumar(Slightly)
##
CC = gcc
# CXX = /usr/bin/g++
CXX = /usr/bin/g++-11
OR_LIBS = -L./lib -lortools

CFLAGS = --std=c++17 -W -Wall -Wno-sign-compare -O4 -pipe -mmmx -msse -msse2 -msse3 -g -Iinclude/ortools -Iinclude -I. # -mcmodel=medium
# CFLAGS = 
OR_CFLAGS = -v -fPIC -std=c++17 -O4 -DNDEBUG -Iinclude/ortools -Iinclude -I. -g -DARCH_K8 -Wno-deprecated -DUSE_BOP -DUSE_GLOP -DUSE_CBC -DUSE_CLP -DUSE_SCIP
OR_TOOLS_LNK = -Llib -lortools

LDFLAGS = -Wl,-rpath,@loader_path -Wl,-rpath,@loader_path/../lib -lz -lglog
MEM = -D WATCH_MEM

CLUS_INC_DIR = include/clustering
FESIF_INCLUDE_DIR = $(CLUS_INC_DIR)/fesif
CLUS_SRC_DIR = src/clustering
FESIF_SRC_DIR = $(CLUS_SRC_DIR)/fesif

CLARKE_INCLUDE_DIR = src/clustering/Clarke
TSP_INCLUDE_DIR = src/routeplan
OPT_INCLUDE_DIR = src
OPT_HEADER_DIR = include
HGS_INCLUDE_DIR = $(CLUS_INC_DIR)/HGS
HGS_SRC_DIR = $(CLUS_SRC_DIR)/HGS

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

# SelfClustering Algorithm
cluster.o : 
	$(CXX) $(CFLAGS) -c src/clustering/selfClustering/cluster.cpp $(LIBS)

TSP_CK.o :
	$(CXX) $(CFLAGS) -c src/routeplan/TSP_CK.cpp $(LIBS)

clarke.o : 
	$(CXX) $(CFLAGS) -c src/clustering/Clarke/clarke.cpp $(LIBS)
AlgorithmParameters.o: $(HGS_SRC_DIR)/AlgorithmParameters.cpp
	$(CXX)  -fPIC -c $(HGS_SRC_DIR)/AlgorithmParameters.cpp 
C_Interface.o :  $(HGS_SRC_DIR)/C_Interface.cpp 
	$(CXX)  -fPIC -c $(HGS_SRC_DIR)/C_Interface.cpp 
Params.o : $(HGS_SRC_DIR)/Params.cpp 
	$(CXX)  -fPIC -c $(HGS_SRC_DIR)/Params.cpp 
Individual.o : $(HGS_SRC_DIR)/Params.cpp 
	$(CXX)  -fPIC -c $(HGS_SRC_DIR)/Individual.cpp 
LocalSearch.o : $(HGS_SRC_DIR)/LocalSearch.cpp 
	$(CXX)  -fPIC -c $(HGS_SRC_DIR)/LocalSearch.cpp 
Population.o : $(HGS_SRC_DIR)/Population.cpp 
	$(CXX)  -fPIC -c $(HGS_SRC_DIR)/Population.cpp 
Split.o : $(HGS_SRC_DIR)/Split.cpp 
	$(CXX)  -fPIC -c $(HGS_SRC_DIR)/Split.cpp 
Genetic.o : $(HGS_SRC_DIR)/Genetic.cpp
	$(CXX)  -fPIC -c $(HGS_SRC_DIR)/Genetic.cpp
InstanceCVRPLIB.o : $(HGS_SRC_DIR)/InstanceCVRPLIB.cpp 
	$(CXX)  -fPIC -c $(HGS_SRC_DIR)/InstanceCVRPLIB.cpp 
HGS.o: $(HGS_SRC_DIR)/HGS.cpp
	$(CXX) $(CFLAGS) -fPIC -c $(HGS_SRC_DIR)/HGS.cpp
libHGS.so : AlgorithmParameters.o C_Interface.o Params.o Individual.o LocalSearch.o Population.o Split.o Genetic.o InstanceCVRPLIB.o HGS.o
	$(CXX)  -shared -fPIC -o ./lib/libHGS.so AlgorithmParameters.o C_Interface.o Params.o Individual.o LocalSearch.o Population.o Split.o Genetic.o InstanceCVRPLIB.o HGS.o
# Build the executable
Integrate: main.cpp fesif.o TSP_OR.o EB-AFIT.o Optimiser.o $(CLARKE_INCLUDE_DIR)/clarke.cpp $(CLARKE_INCLUDE_DIR)/clarke.hpp $(TSP_INCLUDE_DIR)/TSP.cpp $(TSP_INCLUDE_DIR)/tsp.h $(TSP_INCLUDE_DIR)/TSP_LK.cpp $(RP_INC_DIR)/TSP_LK.hpp libHGS.so
	/usr/bin/g++-11 --std=c++17 -W -Wall -Wno-sign-compare -O4 -pipe -mmmx -msse -msse2 -msse3 -g -Iinclude/ortools -Iinclude -I. global.o HST.o utils.o FESIF.o TSP_OR.o EB-AFIT.o Optimiser.o  main.cpp $(CLARKE_INCLUDE_DIR)/clarke.cpp $(TSP_INCLUDE_DIR)/TSP.cpp $(TSP_INCLUDE_DIR)/TSP_LK.cpp -o Integrate -Llib  -lortools -lHGS
AlgorithmParameters.o: $(HGS_SRC_DIR)/AlgorithmParameters.cpp
	$(CXX) $(CFLAGS) -c $(HGS_SRC_DIR)/AlgorithmParameters.cpp $(LIBS)
C_Interface.o :  $(HGS_SRC_DIR)/C_Interface.cpp AlgorithmParameters.o
	$(CXX) $(CFLAGS) -c $(HGS_SRC_DIR)/C_Interface.cpp $(LIBS)
Params.o : $(HGS_SRC_DIR)/Params.cpp AlgorithmParameters.o
	$(CXX) $(CFLAGS) -c $(HGS_SRC_DIR)/Params.cpp $(LIBS)
Individual.o : $(HGS_SRC_DIR)/Params.cpp Params.o
	$(CXX) $(CFLAGS) -c $(HGS_SRC_DIR)/Individual.cpp $(LIBS)
LocalSearch.o : $(HGS_SRC_DIR)/LocalSearch.cpp Individual.o
	$(CXX) $(CFLAGS) -c $(HGS_SRC_DIR)/LocalSearch.cpp $(LIBS)
Population.o : $(HGS_SRC_DIR)/Population.cpp Individual.o LocalSearch.o
	$(CXX) $(CFLAGS) -c $(HGS_SRC_DIR)/Population.cpp $(LIBS)
Split.o : $(HGS_SRC_DIR)/Split.cpp Individual.o Params.o
	$(CXX) $(CFLAGS) -c $(HGS_SRC_DIR)/Split.cpp $(LIBS)
Genetic.o : $(HGS_SRC_DIR)/Genetic.cpp Individual.o Population.o
	$(CXX) $(CFLAGS) -c $(HGS_SRC_DIR)/Population.cpp $(LIBS)
InstanceCVRPLIB.o : $(HGS_SRC_DIR)/InstanceCVRPLIB.cpp 
	$(CXX) $(CFLAGS) -c $(HGS_SRC_DIR)/InstanceCVRPLIB.cpp $(LIBS)
HGS.o: AlgorithmParameters.o C_Interface.o Params.o Individual.o LocalSearch.o Population.o Split.o Genetic.o InstanceCVRPLIB.o 
	$(CXX) $(CFLAGS) -c $(HGS_SRC_DIR)/HGS.cpp
# Build the executable
Integrate: main.cpp fesif.o TSP_OR.o EB-AFIT.o HGS.o Optimiser.o $(CLARKE_INCLUDE_DIR)/clarke.cpp $(CLARKE_INCLUDE_DIR)/clarke.hpp $(TSP_INCLUDE_DIR)/TSP.cpp $(TSP_INCLUDE_DIR)/tsp.h $(TSP_INCLUDE_DIR)/TSP_LK.cpp $(RP_INC_DIR)/TSP_LK.hpp $(HGS_SRC_DIR)/HGS.cpp
	/usr/bin/g++-11 --std=c++17 -W -Wall -Wno-sign-compare -O4 -pipe -mmmx -msse -msse2 -msse3 -g -Iinclude/ortools -Iinclude -I.  -o main global.o HST.o utils.o FESIF.o TSP_OR.o EB-AFIT.o Optimiser.o HGS.o main.cpp $(CLARKE_INCLUDE_DIR)/clarke.cpp $(TSP_INCLUDE_DIR)/TSP.cpp $(TSP_INCLUDE_DIR)/TSP_LK.cpp -o Integrate -L./lib -Llib -lortools


# Build the executable
main: main.cpp fesif.o TSP_LK.o TSP_OR.o EB-AFIT.o Optimiser.o cluster.o TSP_CK.o clarke.o
	/usr/bin/g++-11 --std=c++17 -W -Wall -Wno-sign-compare -O4 -pipe -mmmx -msse -msse2 -msse3 -g -Iinclude/ortools -Iinclude -I.  -o main global.o HST.o utils.o FESIF.o TSP_OR.o EB-AFIT.o TSP_LK.o cluster.o Optimiser.o TSP_CK.o clarke.o main.cpp -L./lib -Llib -lortools
	
.PHONY: distclean
distclean:
		-@rm *.o *.gcno *~ 2> /dev/null || true
		-@rm fesif chst 2> /dev/null || true
		rm main FESIF*
		rm Integrate
obj_remove:
	-@rm *.o *.gcno *.so *~ 2> /dev/null || true
# Integrate: main.cpp  $(OPT_INCLUDE_DIR)/Optimiser.cpp $(OPT_HEADER_DIR)/Optimiser.hpp
# 	$(CXX) $(CFLAGS) main.cpp  $(OPT_INCLUDE_DIR)/Optimiser.cpp -o Integrate



