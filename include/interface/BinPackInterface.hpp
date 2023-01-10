/**
 * BinPackInterface.hpp
 * ~Archisman Pathak
**/
#ifndef __BINPACK_INTERFACE__
#define __BINPACK_INTERFACE__

#include<vector>
#include<fstream>
#include<iostream>
#include"../datastructures.hpp"

using namespace std;
class BinPackInterface{
    public:
        BinPackInterface() {
            cost = 0;
        }
        // store the computed cluster in clusters 
        virtual void BinPack(vector<item> cluster, Bin b) = 0;
        // calculate the cost of the cluster computed 
        virtual float CalculateCost() = 0; 

        void PrintPackedData()
        {
            cout<<"Printing the packing -- ";
            for(auto &it : packed_items)
            {
                it.print(); cout<<"\n";
            }
            cout<<"\n";
            return;
        }

        void PrintClustersToFile(string filename)
        {
            std::ofstream out(filename);
            int32_t i=0;
            out<<"The packing -- " << i++;
            for(auto &it : packed_items)
            {
                it.printToFile(out); cout<<"\n";
            }
            cout<<"\n";
        }

        vector<item> GetPackaging(){
            return packed_items;
        }

        float GetPackagingCost(){
            return cost;
        }

    private:
        vector<item> packed_items;
        float cost;

};

#endif 