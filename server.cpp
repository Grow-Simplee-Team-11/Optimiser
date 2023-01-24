#include<iostream>
#include<vector>
#include "./include/clustering/fesif/fesif.hpp"
#include "./include/clustering/HGS/HGS.hpp"
#include "./src/clustering/Clarke/clarke.hpp"


#include "./include/routeplan/TSP_OR.hpp"
#include "./include/routeplan/TSP_LK.hpp"
#include "./include/clustering/HGS/HGS.hpp"
#include "./src/routeplan/tsp.h"

#include "./include/binpack/EB_AFIT.hpp"
#include "./include/Optimiser.hpp"

#include <grpcpp/grpcpp.h>

#include "main.pb.h"
#include "main.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using optimiser::Package;
using optimiser::Bin;
using optimiser::Dimension;
using optimiser::Coordinate;
using optimiser::Position;
using optimiser::Cluster;
using optimiser::OptimiserRequest;
using optimiser::OptimiserResponse;
using optimiser::optimiser;

class DataModel{
	public:
	Coordinate warehouse;
	Bin bin;
	vector<item> packages;
	int numRiders;
    DataModel(){
        warehouse.latitude = 0;
        warehouse.longitude = 0;
        packages = vector<item>;
        bin = Bin();
    }
};
DataModel getData(const OptimiserRequest *request){
    DataModel dm;
    dm.numRiders = request->riders();
    dm.warehouse.latitude = request->warehouse().latitude();
    dm.warehouse.longitude = request->warehouse().longitude();
    for(Package &package : request->packages()){
        item i;
        i.id = package.id();
        i.coordinate.latitude = package.coordinates().latitude();
        i.coordinate.longitude = package.coordinates().longitude();
        i.weight = packages.weight();
        i.size.height = packages.size().height();
        i.size.width = packages.size().width();
        i.size.length = packages.size().length();
        i.volume = i.getVolume();
        dm.packages.push_back(i);
    }
    dm.bin.capacity = request->bin().capacity();
    dm.bin.size.height = request->bin().height();
    dm.bin.size.width = request->bin().width();
    dm.bin.size.length = request->bin().length();
    return dm;
}
void setData(Optimiser &optim,OptimiserResponse *reply){

}
class OptimiserServiceImpl final : public optimiser::Service
{
    Status StartService(ServerContext *context, const OptimiserRequest *request,
                   OptimiserResponse *reply) override
    {
        std::cout << "Received request" << std::endl;
        (*reply) = OptimiserResponse();
        RoutePlanInterface* rp = new TSP_OR(EUCLIDEAN);
    	ClusteringInterface* cls = new HGS(EUCLIDEAN);
	    BinPackInterface* bp =  new EB_AFIT;

        DataModel dm = getData(request);

        bool verbose = true;
	    bool logToFile = true;
	    string logFileName = "FESIF_TSP_LK.txt";
        Optimizer optim(rp, cls, bp, dm.packages, dm.warehouse, dm.numRiders, dm.bin, logFileName, verbose, logToFile);
        optim.optimize();

        vector<float> rcosts = optim.GetRoutingCost();
        float total_cost = 0;
        for(auto &x : rcosts)
        {
            total_cost+=x;
        }	

        if(verbose)
            std::cout<<"\nTotal Cost for routing: "<<total_cost<<" km"<<std::endl;
        
        if(logToFile)
        {
            std::ofstream out(logFileName, std::ios_base::app);
            out<<"\nTotal Cost for routing: "<<total_cost<<" km"<<std::endl;
        }
        setData(optim,reply);
        return Status::OK;
    }
};

void RunServer(const std::string &port)
{
    std::string server_address("0.0.0.0:" + port);
    OptimiserServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

int main(int argc, char **argv)
{
    RunServer("50051");
    return 0;
}
