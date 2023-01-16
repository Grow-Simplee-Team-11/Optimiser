#include "./clarke.hpp"

// extern double toRadians(const double degree){}
// extern double Dist(Coordinate &c1, Coordinate &c2){}
double toRadians(const double degree){
    double one_deg = (M_PI) / 180;
    return (one_deg * degree);
}
double Dist(Coordinate &c1,Coordinate &c2){
    double lat1 = toRadians(c1.latitude);
    double long1 = toRadians(c1.longitude);
    double lat2 = toRadians(c2.latitude);
    double long2 = toRadians(c2.longitude);

    double dlong = long2 - long1;
    double dlat = lat2 - lat1;

    double ans = pow(sin(dlat / 2), 2) +
                          cos(lat1) * cos(lat2) *
                          pow(sin(dlong / 2), 2);
 
    ans = 2 * asin(sqrt(ans));
    ans = ans * R;
    return ans;
}

Cluster::Cluster(){
        p1 = 0;
        p2 = 0;
        weight = 0;
        volume = 0;
        rank = 0;
        distance = 0;
}
void Clarke::ComputeClusters(vector<item> &packages, Coordinate warehouse, int numRiders, Bin b){
    this->packages = packages;
    this->warehouse = warehouse;
    this->numPackages = packages.size();
    this->numRiders = numRiders;
    this->b = b;
    this->distThresh = 22;
    this->parent.resize(this->numPackages);
    this->Clusters.resize(this->numPackages);
    solve();
}
void Clarke::test(){
    cout<<"Hello I exist";
}
double Clarke::depotDist(Coordinate &c){
    return Dist(c,warehouse);
}
double Clarke::compute_savings(item& item1,item& item2){
    return depotDist(item1.coordinate) + depotDist(item2.coordinate) - Dist(item1.coordinate,item2.coordinate);
}
void Clarke::create_pq(){
    q = priority_queue<pair<double,pair<int,int>>> ();
    for(int i = 0;i < numPackages;i++){
        for(int j = 0; j < numPackages;j++){
            double savings = compute_savings(packages[i],packages[j]);
            q.push({savings,{i,j}});
        }
    }
}

void Clarke::make_set(int v){
    parent[v] = v;
    Clusters[v] = initCluster(v);
}
int Clarke::find_set(int v){
    if(v == parent[v])
        return v;
    else return parent[v] = find_set(parent[v]);
}
Cluster Clarke::initCluster(int v){
    Cluster c;
        c.p1 = v;
        c.p2 = v;
        c.weight = packages[v].weight;
        c.volume = packages[v].volume;
        c.rank = 1;
        c.distance = 0;
        return c;
}
void Clarke::union_sets(int a,int b){
    int cluster_a = find_set(a);
    int cluster_b = find_set(b);
    if(a != Clusters[cluster_a].p1 && a !=Clusters[cluster_a].p2){
        return;
    }   
    if(b != Clusters[cluster_b].p1 && b !=Clusters[cluster_b].p2){
        return;
    }
    if(Clusters[cluster_a].volume + Clusters[cluster_b].volume > (this->b).getVolume() || Clusters[cluster_a].weight + Clusters[cluster_b].weight > (this->b).capacity){
        return;
    }
    if(cluster_a==cluster_b)
        return;
    int newp1,newp2;
    if(Clusters[cluster_a].p1==a){
        newp1 = Clusters[cluster_a].p2;
    }
    else{
        newp1 = Clusters[cluster_a].p1;
    }
    if(Clusters[cluster_b].p1==b){
        newp2 = Clusters[cluster_b].p2;
    }
    else{
        newp2 = Clusters[cluster_b].p1;
    }
    if(Dist(packages[a].coordinate,packages[b].coordinate) + Clusters[cluster_b].distance + Clusters[cluster_a].distance + depotDist(packages[newp1].coordinate) + depotDist(packages[newp2].coordinate) > distThresh){
        return;
    }

    if (cluster_a != cluster_b){
        if (Clusters[cluster_a].rank < Clusters[cluster_b].rank)
            swap(cluster_a,cluster_b);
        parent[cluster_b] = cluster_a;
        Clusters[cluster_a].rank += Clusters[cluster_b].rank;
        Clusters[cluster_a].weight+= Clusters[cluster_b].weight;
        Clusters[cluster_a].volume+= Clusters[cluster_b].volume;
        Clusters[cluster_a].distance += Dist(packages[a].coordinate,packages[b].coordinate) + Clusters[cluster_b].distance;        
        Clusters[cluster_a].p1 = newp1;
        Clusters[cluster_a].p2 = newp2;
    }
}
void Clarke::solve(){
    map<int,vector<item>> cluster_list;
    create_pq();
    for(int i = 0;i < numPackages;i++)
        make_set(i);
    while(!q.empty()){
        auto it = q.top();
        q.pop();
        int p1 = it.second.first;
        int p2 = it.second.second;
        union_sets(p1,p2);
    }
    for(int i = 0;i<numPackages;i++){
        cluster_list[find_set(i)].push_back(packages[i]);
    }
    for(auto it = cluster_list.begin();it != cluster_list.end();it++){
        clusters.push_back(it->second);
    } 
    for(int i = 0 ;i < clusters.size();i++){
        cout<< "Cluster: "<<i+1<<endl;
        for(int j = 0 ;j < clusters[i].size();j++){
            cout<<clusters[i][j].coordinate.latitude<<" "<<clusters[i][j].coordinate.longitude<<endl;
        }
        cout<<endl;
    }
}
void Clarke::CalculateCost(){
    return;
}