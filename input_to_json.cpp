#include<bits/stdc++.h>
#include "include/datastructures.hpp"

using namespace std;

int main(){
    double a,b;
    cin>>a>>b;
    Bin bin;
    cin>>bin.size.height>>bin.size.width>>bin.size.length;
    int n;
    cin>>n;
    vector<item> items;
    for(int i =0;i<n;i++){
        item it;
        cin>>it.coordinate.latitude>>it.coordinate.longitude>>it.size.height>>it.size.width>>it.size.length;
        it.volume = it.getVolume();
        cin>>it.weight;
        cin>>it.time;
        items.push_back(it);
    }
    cout<<"starting"<<endl;
    ofstream output;
    output.open("input.json");
    output<<"{"<<endl;
    output<<"\"packages\": ["<<endl;
    for(int i =0;i<n;i++){
        output<<"{"<<endl;
        output<<"\"id\": "<<i+1<<","<<endl;
        output<<"\"size\": {"<<endl;
        output<<"\"length\": "<<items[i].size.length<<","<<endl;
        output<<"\"width\": "<<items[i].size.width<<","<<endl;
        output<<"\"height\": "<<items[i].size.height<<endl;
        output<<"},"<<endl;
        output<<"\"coordinates\": {"<<endl;
        output<<"\"latitude\": "<<(int)(items[i].coordinate.latitude*1e6)<<","<<endl;
        output<<"\"longitude\": "<<(int)(items[i].coordinate.longitude*1e6)<<endl;
        output<<"},"<<endl;
        
        output<<"\"weight\": "<<items[i].weight<<endl;
        if(i==n-1){
            output<<"}"<<endl;
        }
        else{
            output<<"},"<<endl;
        }
    }
    output<<"],"<<endl;
    output<<"\"bin\": {"<<endl;
    output<<"\"size\": "<<endl;
    output<<"{"<<endl;
    output<<"\"length\": "<<bin.size.length<<","<<endl;
    output<<"\"width\": "<<bin.size.width<<","<<endl;
    output<<"\"height\": "<<bin.size.height<<endl;
    output<<"},"<<endl;
    output<<"\"capacity\": "<<512000<<endl;
    output<<"},"<<endl;
    output<<"\"riders\": 10,"<<endl;
    output<<"\"warehouse\": {"<<endl;
    output<<"\"latitude\": "<<(int)(a*1e6)<<","<<endl;
    output<<"\"longitude\": "<<(int)(b*1e6)<<endl;
    output<<"}"<<endl;


    output<<"}"<<endl;
    output.close();

    return 0;
}