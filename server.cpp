#include<iostream>
#include<vector>
#include "./include/clustering/fesif/fesif.hpp"
#include "./include/clustering/HGS/HGS.hpp"
#include "./include/clustering/Clarke/clarke.hpp"


#include "./include/routeplan/TSP_OR.hpp"
#include "./include/routeplan/TSP_LK.hpp"
#include "./include/clustering/HGS/HGS.hpp"
#include "./include/routeplan/TSP_CK.hpp"

#include "./include/binpack/EB_AFIT.hpp"
#include "./include/Optimiser.hpp"

#include <grpcpp/grpcpp.h>

#include "main.pb.h"
#include "main.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using optimizer::Package;
using optimizer::ResponsePackage;
// using optimizer::Bin;
// using optimizer::Dimension;
// using optimizer::Coordinate;
// using optimizer::Position;
// using optimizer::Cluster;
using optimizer::OptimizerRequest;
using optimizer::OptimizerResponse;
// using optimizer::optimizer;

class DataModel{
	public:
	Coordinate warehouse;
	Bin bin;
	vector<item> packages;
	int numRiders;
    DataModel(){
        warehouse.latitude = 0;
        warehouse.longitude = 0;
        packages = vector<item>();
        bin = Bin();
    }
};
DataModel getData(const OptimizerRequest *request){
    DataModel dm;
    dm.numRiders = request->riders();
    dm.warehouse.latitude = request->warehouse().latitude()/1e6;
    dm.warehouse.longitude = request->warehouse().longitude()/1e6;
    for(Package package : request->packages()){
        item i;
        i.id = package.id();
        i.coordinate.latitude = package.coordinates().latitude()/1e6;
        i.coordinate.longitude = package.coordinates().longitude()/1e6;
        i.weight = package.weight();
        i.size.height = package.size().height();
        i.size.width = package.size().width();
        i.size.length = package.size().length();
        i.volume = i.getVolume();
        dm.packages.push_back(i);
    }
    dm.bin.capacity = request->bin().capacity();
    dm.bin.size.height = request->bin().size().height();
    dm.bin.size.width = request->bin().size().width();
    dm.bin.size.length = request->bin().size().length();
    return dm;
}
void setData(Optimizer &optim,OptimizerResponse *reply){
    int numClusters = optim.getNumClusters();
    for(int i = 0;i < numClusters;i++){
        reply->add_clusters();
        vector<item> cluster = optim.GetPackagingForCluster(i);
        int numPackages = cluster.size();
        for(int j = 0;j < numPackages;j++){
            ResponsePackage pkg;
            pkg.set_id(cluster[j].id);
            optimizer::Position pos;
            pos.set_x(cluster[j].position.x);
            pos.set_y(cluster[j].position.y);
            pos.set_z(cluster[j].position.z);
            pos.set_length(cluster[j].size.length);
            pos.set_breadth(cluster[j].size.width);
            pos.set_height(cluster[j].size.height);
            // pkg.set_position(pos);
            *(pkg.mutable_position()) = pos;
            reply->mutable_clusters(i)->add_packages();
            *(reply->mutable_clusters(i)->mutable_packages(j)) = pkg;
        }
    }
}

class OptimizerServiceImpl final : public optimizer::optimizer::Service
{
    Status StartService(ServerContext *context, const OptimizerRequest *request,
                   OptimizerResponse *reply) override
    {
        std::cout << "Received request" << std::endl;
        (*reply) = OptimizerResponse();
        // RoutePlanInterface* rp = new TSP_OR(EUCLIDEAN);
        RoutePlanInterface* rp = new TSP_OR(EUCLIDEAN);
    	ClusteringInterface* cls = new HGS(EUCLIDEAN);
	    BinPackInterface* bp =  new EB_AFIT;
        cout << "I am here" << endl;
        DataModel dm = getData(request);

        bool verbose = true;
	    bool logToFile = true;
	    string logFileName = "FESIF_TSP_LK.txt";
        Optimizer optim(rp, cls, bp, dm.packages, dm.warehouse, dm.numRiders, dm.bin, logFileName, verbose, logToFile);
        optim.optimize();


        for(auto &x: optim.clusters)
        {
            std::cout<<x.size.length<<" "<<x.size.breadth<<" "<<x.size.height<<std::endl;
        }
        
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
    OptimizerServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service); 
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

int32_t main(int argc, char **argv)
{
    RunServer("50051");
    return 0;
}
