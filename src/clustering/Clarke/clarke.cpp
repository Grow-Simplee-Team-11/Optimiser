#include "../../include/clustering/Clarke/clarke.hpp"
/**
 * @brief Implemented merge sort (sorting pairs of clusters based on how likely they should be merged) using custom comparator
 * 
 * @param final_points --> list of pairs (i,j) corresponding to the indexes in clusters vector
 * @param l left bound for sorting
 * @param r right bound for sorting
 */
void Clarke::merge_sort(vector<pair<int,int>> final_points, int l , int r){
    if(r<=l){
        return;
    }
    merge_sort(final_points, l, (l+r)/2);
    merge_sort(final_points, (l+r)/2+1,r);

    int i=l,j=(l+r)/2+1;
    vector<pair<int,int>> temp;
    while(i<(l+r)/2+1 && j< r+1){
        if(cmp(final_points[i],final_points[j])){
            temp.push_back(final_points[i]);
            i++;
        }
        else{
            temp.push_back(final_points[j]);
            j++;
        }
    }
    while(i < (l+r)/2+1){
        temp.push_back(final_points[i]);
        i++;
    }
    while(j < r+1){
        temp.push_back(final_points[j]);
        j++;
    }
    for(int i=l;i<=r;i++)
        final_points[i]=temp[i-l];
    return;
}
/**
 * @brief Constructor for a Cluster object
 * 
 * @param p1 --> index of left endpoint in the cluster
 * @param p2 --> index of right enpoint in the cluster
 * @param weight --> total weight of the cluster
 * @param volume --> total volume of the cluster
 * @param rank --> rank of the cluster (DSU)
 * @param distance --> total path length of the cluster
 */
Cluster::Cluster(){
        p1 = 0;
        p2 = 0;
        weight = 0;
        volume = 0;
        rank = 0;
        distance = 0;
}
/**
 * @brief Compute Clusters (implementation of the Clarke and Wright savings heuristic)
 * 
 * @param packages list of items to be delivered
 * @param warehouse Depot Coordinate
 * @param numRiders Number of riders available for delivery
 * @param b Bag dimensions (for volume constraint)
 */
void Clarke::ComputeClusters(vector<item> &packages, Coordinate warehouse, int numRiders, Bin b){
    clusters.clear();
    this->packages = packages;
    this->warehouse = warehouse;
    this->numPackages = packages.size();
    this->numRiders = numRiders;
    this->b = b;
    this->distThresh = 50000;
    this->parent.resize(this->numPackages);
    this->Clusters.resize(this->numPackages);
    solve();
}
void Clarke::test(){
    cout<<"Hello I exist";
}
/**
 * @brief Utility Function for computing distance of a location from the depot
 * 
 * @param c location in latitude and longitude
 * @return double Distance from the depot
 */
double Clarke::depotDist(Coordinate &c){
    return Dist(c,warehouse);
}
/**
 * @brief Utility Function for computing savings (heuristic) if the two items belong to a single cluster
 * 
 * @param item1 first of the 2 items to be merged into a cluster
 * @param item2 second of the 2 items to be merged into a cluster
 * @return double Savings on distance if the two items are merged into a single cluster
 */
double Clarke::compute_savings(item& item1,item& item2){
    return depotDist(item1.coordinate) + depotDist(item2.coordinate) - Dist(item1.coordinate,item2.coordinate) + abs(item1.time -item2.time);
}
/**
 * @brief Function to create priority queue of all possible paris of items and their corresponding savings on merge
 * 
 */
void Clarke::create_pq(){
    q = priority_queue<pair<double,pair<int,int>>> ();
    for(int i = 0;i < numPackages;i++){
        for(int j = 0; j < numPackages;j++){
            double savings = compute_savings(packages[i],packages[j]);
            q.push({savings,{i,j}});
        }
    }
}
/**
 * @brief Initialize DSU
 * 
 * @param v index at which set is to be initialized
 */
void Clarke::make_set(int v){
    parent[v] = v;
    Clusters[v] = initCluster(v);
}
/**
 * @brief Find the parent of a set
 * 
 * @param v index of the set
 * @return int index of the parent of the set
 */
int Clarke::find_set(int v){
    if(v == parent[v])
        return v;
    else return parent[v] = find_set(parent[v]);
}
/**
 * @brief Constructor for a Cluster object
 * 
 * @param v 
 * @return Cluster 
 */
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
/**
 * @brief Merge two clusters
 * 
 * @param a index of the first cluster
 * @param b index of the second cluster
 * @param constraints flag to check if the merge violates the volume or weight constraints
 * @return int 1 if merge is successful, 0 if not 
 */
int Clarke::union_sets(int a,int b, bool constraints){
    int cluster_a = find_set(a);
    int cluster_b = find_set(b);
    if(a != Clusters[cluster_a].p1 && a !=Clusters[cluster_a].p2 ){
        return 0; 
    }   
    if(b != Clusters[cluster_b].p1 && b !=Clusters[cluster_b].p2){
        return 0;
    }
    if((Clusters[cluster_a].volume + Clusters[cluster_b].volume > (this->b).getVolume() && constraints) || (Clusters[cluster_a].weight + Clusters[cluster_b].weight > (this->b).capacity && constraints)){
        return 0;
    }
    if(cluster_a==cluster_b)
        return 0;
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
    if(Dist(packages[a].coordinate,packages[b].coordinate) + Clusters[cluster_b].distance + Clusters[cluster_a].distance + depotDist(packages[newp1].coordinate) + depotDist(packages[newp2].coordinate) > distThresh && constraints){
        return 0;
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
    return 1;
}
/**
 * @brief Comparatoe function to evaluate likeliness of a pair of clusters to be merged 
 * 
 * @param a pair of indexes of the clusters to be merged
 * @param b pair of indexes of the clusters to be merged
 * @return true if clusters belonging to a is more likely to be merged with as compared to b
 * @return false if clusters belonging to b is more likely to be merged with as compared to a
 */
bool Clarke::cmp(const pair<int,int> a,const pair<int,int> b){
    int cluster_a1 = find_set(a.first);
    int cluster_a2 = find_set(a.second);
    int cluster_b1 = find_set(b.first);
    int cluster_b2 = find_set(b.second);

    double normaliser_a = sqrt((Clusters[cluster_a1].distance + Clusters[cluster_a2].distance + Dist(packages[a.first].coordinate,packages[a.second].coordinate))*(Clusters[cluster_a1].distance + Clusters[cluster_a2].distance + Dist(packages[a.first].coordinate,packages[a.second].coordinate)) + (Clusters[cluster_a1].volume + Clusters[cluster_a2].volume)*(Clusters[cluster_a1].volume + Clusters[cluster_a2].volume) + (Clusters[cluster_a1].weight + Clusters[cluster_a2].weight)*(Clusters[cluster_a1].weight + Clusters[cluster_a2].weight) +  (Clusters[cluster_a1].rank + Clusters[cluster_a2].rank)*(Clusters[cluster_a1].rank + Clusters[cluster_a2].rank));
    double normaliser_b = sqrt((Clusters[cluster_b1].distance + Clusters[cluster_b2].distance + Dist(packages[b.first].coordinate,packages[b.second].coordinate))*(Clusters[cluster_b1].distance + Clusters[cluster_b2].distance + Dist(packages[b.first].coordinate,packages[b.second].coordinate)) + (Clusters[cluster_b1].volume + Clusters[cluster_b2].volume)*(Clusters[cluster_b1].volume + Clusters[cluster_b2].volume) + (Clusters[cluster_b1].weight + Clusters[cluster_b2].weight)*(Clusters[cluster_b1].weight + Clusters[cluster_b2].weight) +  (Clusters[cluster_b1].rank + Clusters[cluster_b2].rank)*(Clusters[cluster_b1].rank + Clusters[cluster_b2].rank));

    double a_val = (Clusters[cluster_a1].distance + Clusters[cluster_a2].distance - Dist(packages[a.first].coordinate,packages[a.second].coordinate) - (Clusters[cluster_a1].volume + Clusters[cluster_a2].volume) - (Clusters[cluster_a1].weight + Clusters[cluster_a2].weight) -  (Clusters[cluster_a1].rank + Clusters[cluster_a2].rank))/normaliser_a;
    double b_val = (Clusters[cluster_b1].distance + Clusters[cluster_b2].distance - Dist(packages[b.first].coordinate,packages[b.second].coordinate) - (Clusters[cluster_b1].volume + Clusters[cluster_b2].volume) - (Clusters[cluster_b1].weight + Clusters[cluster_b2].weight) -  (Clusters[cluster_b1].rank + Clusters[cluster_b2].rank))/normaliser_b;
    return a_val > b_val;
}
/**
 * @brief Function to consolidate the results of the Clarke Wright Algorithm to limit the number of clusters to number of available riders
 * 
 */
void Clarke::consolodate_further(){
    vector<pair<int,int>> final_points;
    for(int i = 0 ;i < final_Clusters.size(); i++){
        for(int j = 0; j < final_Clusters.size(); j++){
            final_points.push_back({final_Clusters[i].p1,final_Clusters[j].p1});
            final_points.push_back({final_Clusters[i].p1,final_Clusters[j].p2});
            final_points.push_back({final_Clusters[i].p2,final_Clusters[j].p1});
            final_points.push_back({final_Clusters[i].p2,final_Clusters[j].p2});
        }
    }  
    if(final_points.size() <= 0)
        return;
    merge_sort(final_points, 0, final_points.size()-1);
    int numCluster = final_Clusters.size();
    // cout<<"numCluster ======>"<<numCluster<<" numRiders =======>"<<numRiders<<endl;
    while(numCluster > numRiders){
        numCluster -= union_sets(final_points[final_points.size()-1].first,final_points[final_points.size()-1].second,0);
        final_points.pop_back();
        merge_sort(final_points, 0, final_points.size()-1);
    }
    return;
}
/**
 * @brief Compute Clusters (implementation of the Clarke and Wright savings heuristic)
 *  Returns the generated clusters in the final_Clusters vector
 */
void Clarke::solve(){
    map<int,vector<item>> cluster_list;
    create_pq();
    final_Clusters.clear();
    for(int i = 0;i < numPackages;i++)
        make_set(i);
    while(!q.empty()){
        auto it = q.top();
        q.pop();
        int p1 = it.second.first;
        int p2 = it.second.second;
        union_sets(p1,p2,1);
    }
    for(int i = 0;i<numPackages;i++){
        if(cluster_list.find(find_set(i))==cluster_list.end())
            final_Clusters.push_back(Clusters[find_set(i)]);
        cluster_list[find_set(i)].push_back(packages[i]);
    }
    // for(int i= 0 ;i < final_Clusters.size();i++){
    //     cout<<final_Clusters[i].p1<<" "<<final_Clusters[i].p2<<endl;
    // }
    consolodate_further();
    cluster_list.clear();
    set<int> s;
    for(int i = 0;i<numPackages;i++){
        s.insert(find_set(i));
        cluster_list[find_set(i)].push_back(packages[i]);
    }
    ofstream output;
    output.open("./tests/cluster_vol.txt");
    for(auto it = s.begin();it != s.end();it++){
        output << Clusters[*it].weight << endl;
        for(int i = 0;i < cluster_list[*it].size();i++){
            output << cluster_list[*it][i].weight << ' ';
        }
        output << endl;
    }
    output.close();
    for(auto it = cluster_list.begin();it != cluster_list.end();it++){
        clusters.push_back(it->second);
    } 
    // for(int i = 0 ;i < clusters.size();i++){
    //     cout<< "Cluster: "<<i+1<<endl;
    //     for(int j = 0 ;j < clusters[i].size();j++){
    //         cout<<clusters[i][j].coordinate.latitude<<" "<<clusters[i][j].coordinate.longitude<<endl;
    //     }
    //     cout<<endl;
    // }
    
}
void Clarke::CalculateCost(){
    return;
}