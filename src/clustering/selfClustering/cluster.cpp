#include<bits/stdc++.h>
#include "../../../include/clustering/selfClustering/selfClustering.hpp"
using namespace std;

int angularCutMetric = 3;

void createCircle(vector<item>& items) {
    sort(items.begin(), items.end(), [&](item a, item b) {
        return a.polarCoordinate.angle < b.polarCoordinate.angle;
    });
}

vector<vector<item> > createRadialCuts(vector<vector<item> >& angularCuts, Bin deliveryBag) {
    // TODO: Define this max volume and max weight
    float maxVolume = deliveryBag.getVolume(), maxWeight = deliveryBag.getCapacity(); 

    int k = angularCuts.size();
    vector<vector<item> > clusters;
    for(int i=0;i<k;i++) 
    {
        sort(angularCuts[i].begin(), angularCuts[i].end(), [&](item a, item b) {
            return a.polarCoordinate.radius < b.polarCoordinate.radius;
        });
        float totalWeight = 0, totalVolume = 0;
        vector<item> currentCluster;
        currentCluster.push_back(angularCuts[i][0]);
        totalWeight += angularCuts[i][0].weight;
        totalVolume += angularCuts[i][0].size.length * angularCuts[i][0].size.width * angularCuts[i][0].size.height;
        for(int j=1;j<angularCuts[i].size();j++) {
            float tempVolume = totalVolume + angularCuts[i][j].size.length * angularCuts[i][j].size.width * angularCuts[i][j].size.height;
            float tempWeight = totalWeight + angularCuts[i][j].weight; 

            if(tempVolume < maxVolume && tempWeight < maxWeight) {
                currentCluster.push_back(angularCuts[i][j]);
                totalVolume = tempVolume;
                totalWeight = tempWeight;
            }
            else {
                clusters.push_back(currentCluster);
                currentCluster.clear();
                currentCluster.push_back(angularCuts[i][j]);
                totalVolume = angularCuts[i][j].size.length * angularCuts[i][j].size.width * angularCuts[i][j].size.height;
                totalWeight = angularCuts[i][j].weight;
            }
        }
        clusters.push_back(currentCluster);
    }
    return clusters;
}

bool checkClusterValidity(vector<vector<item> >& clusters) {
    return true;
}

vector<vector<item> > createAngularCuts(vector<item>& circle, Bin deliveryBag) {
    vector<vector<item> > cuts;
    int n = circle.size();

    vector<pair<float, int> > angles;
    for(int i=0;i<n-1;i++) {
        angles.push_back(make_pair(circle[(i+1)%n].polarCoordinate.angle-circle[i].polarCoordinate.angle, i));
    }
    angles.push_back(make_pair(2*M_PI - circle[n-1].polarCoordinate.angle+circle[0].polarCoordinate.angle, n-1));

    sort(angles.begin(), angles.end());
    for(int i=0;i<angles.size();i++) {
        cout << angles[i].first << " " << angles[i].second << endl;
    }
    float maxVolume = deliveryBag.getVolume()*SELFCLUSTERING::angularCutMetric, maxWeight = deliveryBag.getCapacity()*angularCutMetric;
    int starting = (angles.back().second+1)%n;
    angles.pop_back();
    vector<item> current;
    float totalVolume = 0, totalWeight = 0;
    for(int i=0;i<n;i++) {
        float tempVolume = totalVolume + circle[(starting+i)%n].getVolume();
        float tempWeight = totalWeight + circle[(starting+i)%n].weight;
        if((starting+i)%n == (angles.back().second+1)%n) {
            cuts.push_back(current);
            cout<<"Cut at: "<<(starting+i)%n<<endl;
            current.clear();
            current.push_back(circle[(starting+i)%n]);
            totalVolume = circle[(starting+i)%n].getVolume();
            totalWeight = circle[(starting+i)%n].weight;
            angles.pop_back();
            continue;
        }
        if(tempVolume < maxVolume && tempWeight < maxWeight) {
            current.push_back(circle[(starting+i)%n]);
            totalVolume = tempVolume;
            totalWeight = tempWeight;
        }
        else {
            cuts.push_back(current);
            cout<<"Cut at: "<<(starting+i)%n<<endl;
            current.clear();
            current.push_back(circle[(starting+i)%n]);
            totalVolume = circle[(starting+i)%n].getVolume();
            totalWeight = circle[(starting+i)%n].weight;
        }
    }
    cuts.push_back(current);
    return cuts;
}

void assignPolarCoordinate(item& temp, Coordinate warehouse) {
    double x = temp.coordinate.longitude - warehouse.longitude;
    double y = temp.coordinate.latitude - warehouse.latitude;
    temp.polarCoordinate.radius = sqrt((temp.coordinate.longitude-warehouse.longitude)*(temp.coordinate.longitude-warehouse.longitude) + (temp.coordinate.latitude-warehouse.latitude)*(temp.coordinate.latitude-warehouse.latitude));
    temp.polarCoordinate.angle = atan2(temp.coordinate.latitude-warehouse.latitude, temp.coordinate.longitude-warehouse.longitude);
    if(y<0) temp.polarCoordinate.angle+=2*M_PI;
    temp.polarCoordinate.angle*=180.0/M_PI;
}

vector<vector<item> > SELFCLUSTERING::calculateCluster(vector<item>& items, Coordinate warehouse, int numberOfRiders, Bin bin) {

    for(int i=0;i<items.size();i++) {
        assignPolarCoordinate(items[i], warehouse);
    }
    createCircle(items);


    vector<vector<item> > angularClusters = createAngularCuts(items, bin);
    vector<vector<item> > clusters = createRadialCuts(angularClusters, bin);

    return clusters;
}

void printClusters(vector<vector<item> >& clusters, vector<item>& items) {
    vector<int>  clusterIndex(items.size());
    cout<<clusters.size()<<endl;
    for(int i=0;i<clusters.size();i++) {
        for(int j=0;j<clusters[i].size();j++) {
            clusterIndex[clusters[i][j].id] = i;
        }
    }
    ofstream myfile;
  	myfile.open ("output.txt");

	for(int i=0;i<clusterIndex.size();i++) {
		myfile<<clusterIndex[i]<<" ";
	}
	myfile<<endl;
	myfile.close();
}