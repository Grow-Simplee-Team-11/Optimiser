##
#	
# GRPC Integrated Makefile
# Author: Archisman Pathak & Aniket Kumar(Slightly)
##

HOST_SYSTEM = $(shell uname | cut -f 1 -d_)
SYSTEM ?= $(HOST_SYSTEM)
CXX = g++
CPPFLAGS += `pkg-config --cflags protobuf grpc`
CXXFLAGS += -std=c++14
ifeq ($(SYSTEM),Darwin)
LDFLAGS += -L/usr/local/lib `pkg-config --libs --static protobuf grpc++`\
					 -pthread\
           -lgrpc++_reflection\
           -ldl
else
LDFLAGS += -L/usr/local/lib `pkg-config --libs --static protobuf grpc++`\
           -pthread\
           -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed\
           -ldl
endif
PROTOC = protoc
GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`

PROTOS_PATH = ./proto
vpath %.proto $(PROTOS_PATH)

CC = gcc
# CXX = /usr/bin/g++
CXX = /usr/bin/g++-11
OR_LIBS = -L./lib -lortools
CFLAGS = --std=c++17 -W -Wall -Wno-sign-compare -O4 -pipe -mmmx -msse -msse2 -msse3 -g -Iinclude/ortools -Iinclude -I. -lpthread # -mcmodel=medium
# CFLAGS = 
OR_CFLAGS = -v -fPIC -std=c++17 -O4 -g -DNDEBUG -Iinclude/ortools -Iinclude -I. -DARCH_K8 -Wno-deprecated -DUSE_BOP -DUSE_GLOP -DUSE_CBC -DUSE_CLP -DUSE_SCIP
OR_TOOLS_LNK = -Llib -lortools -lpthread
# LDFLAGS = -Wl,-rpath,@loader_path -Wl,-rpath,@loader_path/../lib -lz -lglog
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


all: system-check server 
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
	$(CXX)  -shared -fPIC -o libHGS.so AlgorithmParameters.o C_Interface.o Params.o Individual.o LocalSearch.o Population.o Split.o Genetic.o InstanceCVRPLIB.o HGS.o
# Build the executable
server: server.cpp fesif.o TSP_OR.o EB-AFIT.o HGS.o Optimiser.o $(CLARKE_INCLUDE_DIR)/clarke.cpp $(CLARKE_INCLUDE_DIR)/clarke.hpp $(TSP_INCLUDE_DIR)/TSP.cpp $(TSP_INCLUDE_DIR)/tsp.h $(TSP_INCLUDE_DIR)/TSP_LK.cpp $(RP_INC_DIR)/TSP_LK.hpp libHGS.so main.pb.o main.grpc.pb.o server.o
	/usr/bin/g++-11 --std=c++17 -W -Wall -Wno-sign-compare -O4 -pipe -mmmx -msse -msse2 -msse3 -g -Iinclude/ortools -Iinclude -I.  global.o HST.o utils.o FESIF.o TSP_OR.o EB-AFIT.o Optimiser.o main.pb.o main.grpc.pb.o server.cpp $(CLARKE_INCLUDE_DIR)/clarke.cpp $(TSP_INCLUDE_DIR)/TSP.cpp $(TSP_INCLUDE_DIR)/TSP_LK.cpp -o server.o -L./lib -Llib -lortools -L. -lHGS  $(LDFLAGS)


%.grpc.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --grpc_out=. --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<

%.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --cpp_out=. $<


.PHONY: clean
clean:
		-@rm *.o *.gcno *.so  *.pb.cc *.pb.h server *~ 2> /dev/null || true
		-@rm fesif chst 2> /dev/null || true
		
obj_remove:
	-@rm *.o *.gcno *.so *~ 2> /dev/null || true
# Integrate: main.cpp  $(OPT_INCLUDE_DIR)/Optimiser.cpp $(OPT_HEADER_DIR)/Optimiser.hpp
# 	$(CXX) $(CFLAGS) main.cpp  $(OPT_INCLUDE_DIR)/Optimiser.cpp -o Integrate


# The following is to test your system and ensure a smoother experience.
# They are by no means necessary to actually compile a grpc-enabled software.

PROTOC_CMD = which $(PROTOC)
PROTOC_CHECK_CMD = $(PROTOC) --version | grep -q libprotoc.3
PLUGIN_CHECK_CMD = which $(GRPC_CPP_PLUGIN)
HAS_PROTOC = $(shell $(PROTOC_CMD) > /dev/null && echo true || echo false)
ifeq ($(HAS_PROTOC),true)
HAS_VALID_PROTOC = $(shell $(PROTOC_CHECK_CMD) 2> /dev/null && echo true || echo false)
endif
HAS_PLUGIN = $(shell $(PLUGIN_CHECK_CMD) > /dev/null && echo true || echo false)

SYSTEM_OK = false
ifeq ($(HAS_VALID_PROTOC),true)
ifeq ($(HAS_PLUGIN),true)
SYSTEM_OK = true
endif
endif

system-check:
ifneq ($(HAS_VALID_PROTOC),true)
	@echo " DEPENDENCY ERROR"
	@echo
	@echo "You don't have protoc 3.0.0 installed in your path."
	@echo "Please install Google protocol buffers 3.0.0 and its compiler."
	@echo "You can find it here:"
	@echo
	@echo "   https://github.com/protocolbuffers/protobuf/releases/tag/v3.0.0"
	@echo
	@echo "Here is what I get when trying to evaluate your version of protoc:"
	@echo
	-$(PROTOC) --version
	@echo
	@echo
endif
ifneq ($(HAS_PLUGIN),true)
	@echo " DEPENDENCY ERROR"
	@echo
	@echo "You don't have the grpc c++ protobuf plugin installed in your path."
	@echo "Please install grpc. You can find it here:"
	@echo
	@echo "   https://github.com/grpc/grpc"
	@echo
	@echo "Here is what I get when trying to detect if you have the plugin:"
	@echo
	-which $(GRPC_CPP_PLUGIN)
	@echo
	@echo
endif
ifneq ($(SYSTEM_OK),true)
	@false
endif


