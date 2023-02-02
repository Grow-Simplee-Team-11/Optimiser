# Route Optimisation and Planning for Last Mile Delivery

## About The Project

'''
Add Image for the application, may be a running image. 
'''

In this part of our solution we have tackled the problem of Route Planning and Bin Packing for the rider, in which we aim to improve the current system by applying state-of-the-art algorithms to reduce the complexity of work for the Delivery Hub as well as the Riders while making a delivery.

We receive the data about the items to be delivered from our database which consist some basic information about the item and it's delivery location. From this we first try to cluster the items based on their location, through which each cluster will be assigned to a single driver for delivery. After clustering the delivery locations together, we try to find the most optimised path among the points taking into consideration the Estimated Delivery Times, Weather and Traffic so as to improve customer experience with on time deliveries. To make it easy for the delivery rider as well, we provide a efficient bag packing for the delivery bag so as to save time and effort for the driver to search for package at the time of delivery. 

All the algorithms used in our solution are then ensembled together to get the best result among all the possible combinations. Metrics are calculated among the combinations to get the maximum score. The best is then used for final execution. This data is then sent to the front-end to show it to the user.

In addition to all this, we also provide a service for dynamic addition of pickup points in the routes of a particular rider, leading into re-routing of the pre-determined path.
## Getting Started
### Quick Install

Execute the following commands to build and run the docker image `lastmiledelivery/optimizer-server`.

To pull image directly from docker-hub :
```bash
sudo docker pull lastmiledelivery/optimizer-server:latest
```
To run the docker image
```bash
sudo docker run -dp 5000:50051 optimizer-server:latest
```
To run the server - 
```bash
sudo docker logs -f <CONTAINER-ID>
```
The server is now ready to recieve requests at port number `5000` on your local system.

The api request format for the enpoint is given in the `quick installation` section below.
#### Note

The Docker Image uses an Ensemble of all implemented algorithms mentioned above to output the best possible results for the given data. To test the algorithms individually, please install the codebase locally.

Details of the implemented algorithms is available under the `Documentation` section.




## Build From Scratch

We specify detailed instructions on the required libraries and Dependencies needed to build and run the code. We recommend to use a Ubuntu 22.04 machine for all purposes.

### Pre-requisites
```bash
$ sudo apt-get update
$ sudo apt-get install build-essential autoconf git pkg-config automake libtool curl make g++ unzip libcurl4-openssl-dev cmake
```

### Dependencies
This repository depends on mainly two external repositories: 
- OR-tools
- gRPC

#### Note
We install all libraries of OR-Tools and gRPC system-wide. Therefor both of them can be cloned and built anywhere in the system using the instructions given below.

### OR-Tools
To compile and install OR-Tools, we build it from source and install the generated libraries system-wide. Refer to [https://github.com/google/or-tools/blob/stable/cmake/README.md]. We used the following commands to fetch and build the repository
```bash
$ git clone -b main https://github.com/google/or-tools
$ cd or-tools
$ cmake -DCMAKE_CXX_STANDARD=17 -S. -Bbuild -DBUILD_DEPS:BOOL=ON 
$ cd build
$ make -j 4
$ sudo make install
```

### gRPC
gRPC is a modern open source high performance Remote Procedure Call (RPC) framework to connect different services. Refer to [https://github.com/grpc/grpc/blob/master/BUILDING.md]  To clone and build the gRPC repository (v1.50.0), Use the following command:

```bash
$ git clone --recurse-submodules -b v1.50.0 \
  --depth 1 --shallow-submodules https://github.com/grpc/grpc
$ cd grpc
$ mkdir -p cmake/build
$ cd ./cmake/build 
$ cmake -DCMAKE_CXX_STANDARD=17 -DCMAKE_INSTALL_PREFIX=/usr/local \
  -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE -DABSL_PROPAGATE_CXX_STD=TRUE ../.. \
  -DgRPC_INSTALL=ON \                                   
  -DCMAKE_BUILD_TYPE=Release       \                                                
  -DgRPC_ABSL_PROVIDER=package     \                                                
  -DgRPC_CARES_PROVIDER=module   \                                                  
  -DgRPC_PROTOBUF_PROVIDER=package \
  -DgRPC_RE2_PROVIDER=package      \
  -DgRPC_SSL_PROVIDER=module      \ 
  -DgRPC_ZLIB_PROVIDER=package
$ make -j 4
$ sudo make install
```

## Installation

In order to setup a local copy of the project, you can follow the one of the 2 methods listed below. Once the local copy is setup, the steps listed in [Usage](#usage) can be used to interact with the system.

1. `Clone` the repo
   ```sh
   $ git clone https://github.com/Grow-Simplee-KGP/Optimiser
   ```
2. Alternatively, Download a ZIP of the source code from github. `unzip` file to unpack all the files included with the project.
   ```sh
   $ unzip Optimiser-main.zip
   ```
3. Change directory to the `Optimiser` directory
   ```sh
   $ cd Optimiser 
   ```
4. Create a build folder inside the `Optimiser` directory and run cmake to generate the makefile. After running the make command, the executable is created in the 
  ```sh
  $ mkdir -p cmake/build
  $ cd cmake/build
  $ cmake ../..
  $ make -j 4
  ```

5. To run the built binary, run the following command from the `Optimiser` directory:
  ```sh
  $ ./bin/server
  ```
By default the server starts at port `50051` in the local machine.
## Usage

Once the local copy of the project has been setup, follow these steps to interact with the system and run tests on the system

### Interact with the system via GRPC Client

You can create a GRPC request containing the Warehouse location , maximum number of riders and list of packages.

The requests are to be structured according to the `OptimiserRequest` message format specified in `proto/main.proto`.  
Given below is a sample request.json file. You can send this using any grpc client (we recommend postman client).

```json
{
    "packages": [
        {
            "id": 1,
            "size": {"length": 4.24591402225247, "width": 5.52875984930662, "height": 5.7068272249847904},
            "coordinates": {"latitude": 12916375, "longitude": 77649741},
            "weight": 11.978200435741485
        },
        {
            "id": 2,
            "size": {"length": 15.69703671350186, "width": 14.797416561668777, "height": 7.423968512381623},
            "coordinates": {"latitude": 12974678, "longitude": 77604902},
            "weight": 10.81211662971237
        },
        {
            "id": 3,
            "size": {"length": 6.615859132172195, "width": 28.528347665831106, "height": 15.023585167751161},
            "coordinates": {"latitude": 12972718, "longitude": 77635140},
            "weight": 10.512273325983648
        }
    ],
    "bin": {"size": {"length": 80, "width": 80, "height": 80}, "capacity": 512000},
    "riders": 15,
    "warehouse": {"latitude": 12907009, "longitude": 77585678}
}

``` 

On sending the above request an `OptimiserResponse` message packet is obtained which lists all clusters and their routes along with packing position.   
Given below is a sample response. 

```json
{
    "clusters": [
        {
            "packages": [
                {
                    "id": 2,
                    "position": {
                        "x": 0,
                        "y": 0,
                        "z": 0,
                        "length": 15,
                        "breadth": 14,
                        "height": 7
                    }
                },
                {
                    "id": 3,
                    "position": {
                        "x": 0,
                        "y": 0,
                        "z": 14,
                        "length": 28,
                        "breadth": 6,
                        "height": 15
                    }
                },
                {
                    "id": 1,
                    "position": {
                        "x": 15,
                        "y": 0,
                        "z": 0,
                        "length": 5,
                        "breadth": 4,
                        "height": 5
                    }
                }
            ]
        }
    ]
```

*The above response is the result of ensembling across all the algorithms mentioned in Acknowledgements section at the end of this document.*

### Interact with the system via Terminal

Instead of running the GRPC server, for quick and easy testing run the terminal application.  
We currently support two testing methods:   

* Testing against custom input:  
  Run the following command  
  
  `$ ./bin/main <INPUT_FILENAME>.txt`. 

  The input file format is specified in `input_format.txt` in source file. 

* Testing against standard VRP Dataset to compare results with current optimal.  
  Download the VRP dataset from [here](https://www.coin-or.org/SYMPHONY/branchandcut/VRP/data/index.htm). Then run the following command : 

  `$ ./bin/main <INPUT_FILE>.vrp`  
  Here `<INPUT_FILE>.vrp` is the desired VRP input file. 
  


## Project Details

Following are the details of the file structure of this project:

```
.
├── bin
│   └── server
├── include
│   ├── binpack
│   │   └── EB_AFIT.hpp
│   ├── clustering
│   │   ├── Clarke
│   │   ├── fesif
│   │   ├── HGS
│   │   ├── selfClustering
│   │   └── TimeWindow
│   ├── datastructures.hpp
│   ├── DistMatrix.hpp
│   ├── interface
│   │   ├── BinPackInterface.hpp
│   │   ├── ClusteringInterface.hpp
│   │   └── RoutePlanInterface.hpp
│   ├── routeplan
│   │   ├── TSP_CK.hpp
│   │   ├── TSP_LK.hpp
│   │   ├── TSP_OR_EDD.hpp
│   │   └── TSP_OR.hpp
│   └── Optimiser.hpp
├── lib
├── main.cpp
├── README.md
├── server.cpp
├── src
│   ├── binpack
│   │   └── EB-AFIT.cpp
│   ├── clustering
│   │   ├── Clarke
│   │   ├── fesif
│   │   ├── HGS
│   │   ├── selfClustering
│   │   └── TimeWindow
│   ├── DistMatrix.cpp
│   ├── Optimiser.cpp
│   └── routeplan
│       ├── TSP_CK.cpp
│       ├── TSP_DP.cpp
│       ├── TSP_LK.cpp
│       ├── TSP_OR.cpp
│       └── TSP_OR_EDD.cpp
├── tests
│   ├── data_gen.py
│   └── visualise_cluster.py
├── CMakeLists.txt
└── Dockerfile

```

Following are the details of the file structure and their functionalities that are present in this code base.

- **Optimiser/bin** - _This folder contains all the necessary binaries for running servers for all the GRPC calls._
  - `Server` binary - Starts the server through which the backend will interact with this code.
- **Optimiser/include/binpack** - This folder contains all the header files used in the Optimiser code for bag packing.
  - `EB_AFIT.hpp` - Header file for the EB_AFIT algorithm implemented. It consist of all the functions required to run the algorithm in the Optimiser parallely.
- **Optimiser/include/clustering** - This folder contains all the header files used in the Optimiser code for clustering of the location points. 
  - `Clarke` - This folder contains all the header files required to implement the Enhanced Clarke-Wright Algorithm. More about the algorithm is present along with the cpp files of the same.
  - `fesif` - This folder contains all the header files required to implement our fast FESI algorithm to get the clusters for further route planning. 
  - `HGS` - This folder contains all the header files reqiured to implement the Genetic algorithm we are using for clustering and path planning simultaneously. In this folder complete support for our HGS is present with tuned hyper-parameters for the best performance.
  - `selfClustering` - The petal cut header files are present in this folder.
  - `TimeWindow` - This folder contains header files for calculating thread metric and time window of each delivery, which is used to increase the percentage of on time deliveries.
 - **Optimiser/include/datastructures.hpp** - _This folder contains all the data structure definions used in our code. This is done so as to formalise the data structure across all the algorithm, making it easier to ensemble the algorithm and minimise overhead to start each algorithm._
- **Optimiser/include/DistMatrix.hpp** - _The methods distance matrix calculation among different locations are present in this header file. The functions are made modular so as to provide easy access for extensions and in current work._
- **Optimiser/include/interface** - _This folder contains all the interfaces used in the optimiser, this made us to make a clean structure of code so as the code is easily maintainable and extensible. Each of the interface in this folder takes care of one important step of optimisation._
  - `ClusteringInterface.hpp` - All the clustering algorithms classes are extensions of this interface, a common function ComputeClusters is implemented in each of them to provide the clusters from the given location points.
  - `RoutePlanInterface.hpp` - All the route planning algorithms are extensions of this interface, we take the benefit of different Object Oriented Principles in creating such a structure.
  - `BinPackInterface.hpp` - This interface takes care of the Bag Packing part of our solution, all the algorithms implemented for the packing have used this interface. 
- **Optimiser/include/routeplan** - _This folder contains header files for all the route planning algorithms. Here all the algorithms are trying to solve the Travelling Salesman Problem(TSP), as after clustering of locations our problem reduced to a basic TSP with addition constraints of EDD and capacity._
  - `TSP_CK.hpp` - This file consist of all the methods used in the modified Christofides algorithm for solving the TSP. 
  - `TSP_LK.hpp` - TSP solver using Lin-Kernighan has been implemented using this header file. This algorithm has been modified to incorporate the traffic data to get precise results for path planning and good approximation for achieving maximum EDDs
  - `TSP_OR_EDD.hpp` - This header file uses the Google OR tools to get the path in each cluster, in addition to the basic OR tools solution we have also added metric to handle the constraints given in the problem statement to get the required results.
- **Optimiser/include/Optimiser.hpp** - This header file is the heart of our solution. This handles the creation of ensemble among all the above algorithms and then provide the best results among all the combinations. Parallelism is used in this through threading to increase the speed among the algorithms.

- **Optimiser/lib** - This folder consist of all the third party libraries such as Google OR tools and the GRPC header files to get the routing and connect backend with our algorithms respectively. These libraries are natively built so as to provide flexibility among the systems it is compiled on.

- **Optimiser/main.cpp** - This file is the spine of our solution. This handles all the data inputs and the parallel running of the Optimiser class to get the best result among all. This file can be changed to test different algorithms and visulaise the results as well. This file will be similar to server.cpp

- **Optimiser/src/binpack** - The older structure of the src folder is similar to what was in the header file, but this contains implementation of all the methods discussed above. `./binpack` folder consist of the binpacking algorithms implementation.
  - `EB-AFIT.cpp` - The implementation of our modified Bin Packing algorithm which is based on the EB-AFIT algorithm[Bin Packing 2] is done in this file. In this we have added constraints to get the package at top at their respective delivery location.
- **Optimiser/src/clustering** - All the clustering algorithms are implemented in this folder.
    - `Clarke` - As discussed in the include folder, this contains the implementation of our Clarke algorithm which is similar to [Clustering Algorithms 1].  Our algorithm is inspired by Kruskal’s algorithm of finding the Minimum Spanning Tree, in which we first select the most profitable locations, profit being the cost reduced in traveling, and join them if they satisfy the condition of endpoints, i.e. the locations need to be the endpoints of their respective group. 
    - `fesif` - 
    - `selfClustering` - 
    - `HGS` - 
    - `TimeWindow` - 
- **Optimiser/src/DistMatrix.cpp** - 
- **Optimiser/src/Optimser.cpp** - 
- **Optimiser/src/routeplan** - 
  - `TSP_CK.cpp` - 
  - `TSP_LK.cpp` - 
  - `TSP_OR_EDD.cpp` -
- **Optimser/tests** - 
  - `data_gen.py` - 
  - `visualise_cluster.py` - 


Things missing, 
 - test files and inputs.
 - visulaise test
 - metric decider

## Roadmap

- Additional browser support

- Add more integrations


Rohit

## Acknowledgements
​
We list below any external resource we came across to help us build the entire codebase -
### Clustering Algorithms
  1.  [Clarke Wright Algorithm](https://oaji.net/articles/2014/818-1399311943.pdf) - Improved upon the classical clarke wright algorithm to also handle `Estimated Time of Delivery` and added a consolidation function.
  2.  [HGS - Hybrid Genetic Search](https://w1.cirrelt.ca/~vidalt/papers/HGS-CIRRELT-2011.pdf) -A Genetic Algorithm for the CVRP . Added `time expectation` to get a better performing heuristic.
3.  [fESI - Fast Embedding Sorting and Insertion](https://www.vldb.org/pvldb/vol13/p320-zeng.pdf) - A `6ρ Approximation Algorithm` implemented using a binary search strategy. A Hierarchical Search Tree is used as spatial index to embed the cost metric.
​
### Route Planning Algorithms
1. [Christofides Algorithm](https://github.com/sth144/christofides-algorithm-cpp/tree/176e61b2d9ba94344fc6c13a57dc3fee7bf0654d) Industry Standard Approximation Algorithm for Travelling Salesman Problem.
2. [Google OR-Tools](https://developers.google.com/optimization) - Linear Programming Optimization library developed by Google with ensemble of randomization and approximation algorithm for CVRP.
​
### Bin Packing Algorithms
1. [Online 3D Bin Packing Using Algorithm](https://openreview.net/forum?id=bfuGjlCwAq) - Trained RL agent for bin backing which maintains package ordering while packing.
2. [EB-AFIT](https://scholar.afit.edu/cgi/viewcontent.cgi?article=5567&context=etd) - Algorithm for 3D Binpacking with an improved setup to maintain package ordering while having high bin packing efficiency.
### Miscellaneous
1. [Google gRPC](https://grpc.io/docs/what-is-grpc/introduction/)-  RPC server handler for handling client side requests by Google.
2. [Bing MAPS API](https://www.microsoft.com/en-us/maps/choose-your-bing-maps-api) Accessable Microsoft API for computing real world distance matrices.
3. [redis GEOSEARCH](https://redis.io/commands/geosearch/) Efficently finds the closest points using geohashing methods to identify candidtate points for dynamic pickup.
